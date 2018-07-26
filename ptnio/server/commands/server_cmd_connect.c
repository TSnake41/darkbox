/*
    ptnio - Portable TCP Network IO
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

#include "server_cmd_utils.h"

/* Syntax : connect sock_id ip port
   Usage : Connect sock_id to ip:port.
*/
void server_cmd_connect(socket_message msg, znsock client, server_data *data)
{
  if (msg.argc < 4) {
    /* Invalid args */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  char *sock_id = msg.argv[1],
       *ip = msg.argv[2];

  in_port_t port = htons(strtoul(msg.argv[3], NULL, 0));

  id_socket_pair *pair = server_get_pair(data, sock_id, NULL);

  if (pair == NULL) {
    /* No pair */
    send_code(client, CMD_NOT_FOUND);
    return;
  }

  struct sockaddr *addr;
  socklen_t len;

  if (server_make_sockaddr(ip, port, pair->ipv6, &addr, &len)) {
    /* Invalid host */
    send_code(client, CMD_INVALID_HOST);
    return;
  }

  if (connect(pair->socket, addr, len) == -1) {
    /* Unable to connect to host. */
    send_code(client, CMD_NETWORK_ERROR);
    free(addr);
    return;
  }

  send_code(client, CMD_SUCCESS);
  free(addr);
}
