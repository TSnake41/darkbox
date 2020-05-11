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
