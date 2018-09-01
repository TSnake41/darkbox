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

/* Syntax : info sock_id
   Usage : Get informations about socket.
*/
void server_cmd_info(socket_message msg, znsock client, server_data *data)
{
  if (msg.argc < 2) {
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
