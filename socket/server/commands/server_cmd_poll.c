/*
    SockeT - Portable TCP and NMS Network IO interface.
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met :

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the name of Teddy Astie (TSnake41) nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY TEDDY ASTIE AND CONTRIBUTORS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL TEDDY ASTIE AND CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
    LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <string.h>
#include <nms.h>

#include "server_cmd_utils.h"

#if (!defined(WIN32) || defined(FORCE_POLL)) && !defined(FORCE_SELECT)
#define USE_POLL
#else
#define USE_SELECT
#endif

/* Syntax : poll timeout sock_id_1 [...] [sock_id_N]
   Usage :
     Poll for input from sockets using a timeout.
     Returns (by output) the available socket.
*/
void server_cmd_poll(socket_message msg, socket_int client, server_data *data)
{
  if (msg.argc < 2) {
    /* Invalid arguments */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  unsigned int count = msg.argc - 2;
  int timeout = strtol(msg.argv[1], NULL, 0);

  char **ids = msg.argv + 2;

  #ifdef USE_POLL
  struct pollfd fds[count];
  #else
  fd_set set;
  FD_ZERO(&set);

  int fds[count];
  #endif

  /* Fill fds with arguments's sockets. */
  for (size_t i = 0; i < count; i++) {
    id_socket_pair *pair = server_get_pair(data, ids[i], NULL);

    if (pair == NULL) {
      /* Pair not found. */
      send_code(client, CMD_NOT_FOUND);
      return;
    }

    #ifdef USE_POLL
    fds[i].fd = pair->socket;
    fds[i].events = POLLIN;
    #else
    FD_SET(pair->socket, &set);
    fds[i] = pair->socket;
    #endif
  }

  #ifdef USE_POLL
  if (poll(fds, count, timeout) < 0) {
    send_code(client, CMD_NETWORK_ERROR);
    return;
  }
  #else
  struct timeval tv, *tv_ptr = NULL;

  if (timeout >= 0) {
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    tv_ptr = &tv;
  }

  if (select(count, &set, NULL, NULL, tv_ptr) < 0) {
    send_code(client, CMD_NETWORK_ERROR);
    return;
  }
  #endif

  send_code(client, CMD_SUCCESS);

  /* Check fds */
  for (size_t i = 0; i < count; i++) {
    #ifdef USE_POLL
    if (fds[i].revents & POLLIN) {
    #else
    if (FD_ISSET(fds[i], &set)) {
    #endif
      /* Send id using nms. */
      char *id = ids[i];
      char new_str[strlen(id) + 2];
      sprintf(new_str, "%s\n", id);
      nms_send(client, new_str, strlen(new_str));
    }
  }

  /* Send zero-size packet (to terminate connection). */
  nms_send(client, NULL, 0);
}
