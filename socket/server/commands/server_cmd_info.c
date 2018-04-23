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

/* Syntax : info sock_id
   Usage : Get informations about socket.
*/
void server_cmd_info(socket_message msg, socket_int client, server_data *data)
{
  if (msg.argc != 2) {
    /* Invalid arguments */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  id_socket_pair *pair = server_get_pair(data, msg.argv[1], NULL);

  if (pair == NULL) {
    /* No pair found */
    send_code(client, CMD_NOT_FOUND);
    return;
  }

  /* Get other informations */
  bool listening = false;
  char local_addr[INET6_ADDRSTRLEN + 6] = { 0 };
  char peer_addr[INET6_ADDRSTRLEN + 6] = { 0 };

  /* Check if socket is listening */
  int v;
  socklen_t len = sizeof(v);

  getsockopt(pair->socket, SOL_SOCKET, SO_ACCEPTCONN, &v, &len);
  listening = v;

  /* Define each ip_port */
  struct sockaddr_storage addr;
  socklen_t addr_len = sizeof(addr);

  /* Get local addr. */
  if (getsockname(pair->socket, (struct sockaddr *)&addr, &addr_len) != -1)
    server_addr_to_str(local_addr, &addr, addr_len);

  /* Get peer addr. */
  if (getpeername(pair->socket, (struct sockaddr *)&addr, &addr_len) != -1)
    server_addr_to_str(peer_addr, &addr, addr_len);

  char *buffer = malloc(0xFFFF);
  if (buffer == NULL) {
    /* No pair found */
    send_code(client, CMD_OUT_OF_MEMORY);
    return;
  }

  unsigned short s = snprintf(buffer, 0xFFFF,
    "id:%s\nlistening:%s\nlocal_addr:%s\npeer_addr:%s\n",
    pair->id, listening ? "true" : "false",
    local_addr, peer_addr);

  /* Send code before data. */
  send_code(client, CMD_SUCCESS);

  nms_send(client, buffer, s);

  free(buffer);
}
