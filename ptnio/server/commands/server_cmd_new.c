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

#include "server_cmd_utils.h"

/* Syntax : new sock_id [IPv6]
   IPv6 : Use IPv6 instead of IPv4.

   Usage : Create a new socket sock_id.
*/
void server_cmd_new(socket_message msg, znsock client, server_data *data)
{
  if (msg.argc < 2) {
    /* Invalid arguments */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  bool overwritten = false;
  unsigned int old_pair_index = 0;

  id_socket_pair *old_pair = server_get_pair(data, msg.argv[1], &old_pair_index);

  /* Remove previous bound socket. */
  if (old_pair != NULL) {
    znsock_close(old_pair->socket, true);
    server_remove_pair(data, old_pair_index);
    overwritten = true;
  }

  bool use_ipv6 = msg.argc > 2 && parse_bool(msg.argv[2]);
  int af = use_ipv6 ? AF_INET6 : AF_INET;

  znsock new_sock = socket(af, SOCK_STREAM, 0);

  if (new_sock == -1) {
    /* Unable to create socket. */
    send_code(client, CMD_NETWORK_ERROR);
    return;
  }

  /* Add the new socket to the list */
  id_socket_pair *pair = malloc(sizeof(id_socket_pair) + strlen(msg.argv[1]) + 1);

  if (pair == NULL) {
    /* Out of memory */
    send_code(client, CMD_OUT_OF_MEMORY);
    znsock_close(new_sock, true);
    return;
  }

  pair->id = get_str(pair);
  strcpy(pair->id, msg.argv[1]);

  pair->socket = new_sock;
  pair->ipv6 = use_ipv6;

  if (server_add_pair(data, pair)) {
    send_code(client, CMD_OUT_OF_MEMORY);
    znsock_close(new_sock, true);
    free(pair);
    return;
  }

  send_code(client, overwritten ? CMD_OVERWRITTEN : CMD_SUCCESS);
}
