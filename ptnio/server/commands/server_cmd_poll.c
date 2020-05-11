/*
  ptnio - Portable TCP Network IO
  Copyright (C) 2017-2018 Teddy ASTIE

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdio.h>
#include <string.h>
#include <nms.h>

#include "server_cmd_utils.h"

/* Syntax : poll timeout sock_id_1 [...] [sock_id_N]
   Usage :
     Poll for input from sockets using a timeout.
     Returns (by output) the available socket.
*/
void server_cmd_poll(socket_message msg, znsock client, server_data *data)
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
    char status = 0;

    #ifdef USE_POLL
    if (fds[i].revents) {
      int fd = fds[i].fd;

      /* Check if socket is listening (a listening socket cannot be disconnected) */
      int listening;
      socklen_t len = sizeof(listening);

      if (getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &listening, &len) == 0) {
        if (!listening && recv(fd, NULL, 0, 0) < 0)
          /* Disconnected/errored peer. */
          status = 'D';
        else
          /* Input available. */
          status = 'I';
      } else {
        /* getsockopt() fails, peer is likely to be disconnected. */
        status = 'D';
      }
    }
    #else
    if (FD_ISSET(fds[i], &set)) {
      /* Check if socket is listening (a listening socket cannot be disconnected) */
      int listening;
      socklen_t len = sizeof(listening);

      if (getsockopt(fds[i], SOL_SOCKET, SO_ACCEPTCONN, &listening, &len) == 0) {
        if (!listening && recv(fds[i], NULL, 0, 0) < 0)
          /* Disconnected/errored peer. */
          status = 'D';
        else
          /* Input available. */
          status = 'I';
      } else {
        /* getsockopt() fails, peer is likely to be disconnected. */
        status = 'D';
      }
    }
    #endif

    if (status) {
      /* Send id using nms. */
      char *id = ids[i];
      char new_str[strlen(id) + 4]; /* 'X ' + id + '\n\0'*/
      sprintf(new_str, "%c %s\n", status, id);
      nms_send(client, new_str, strlen(new_str));
    }
  }

  /* Send zero-size packet (to terminate connection). */
  nms_send(client, NULL, 0);
}
