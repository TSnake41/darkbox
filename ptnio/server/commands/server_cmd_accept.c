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

/* Syntax : accept listner_sock_id [new_sock_id]
   Usage : Accept a pending socket connection of listner_sock_id.
   If new_sock_id is not defined, define id to "IP:PORT".
*/
void server_cmd_accept(socket_message msg, znsock client, server_data *data)
{
  if (msg.argc < 2) {
    /* Invalid arguments */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  bool overwritten = false;
  bool id_arg_defined = msg.argc > 2;

  id_socket_pair *listner_pair = server_get_pair(data, msg.argv[1], NULL);

  if (listner_pair == NULL) {
    /* No pair */
    send_code(client, CMD_NOT_FOUND);
    return;
  }

  struct sockaddr_storage addr = { 0 };
  socklen_t addr_len = sizeof(addr);

  znsock new_socket =
    accept(listner_pair->socket, (struct sockaddr *)&addr, &addr_len);

  if (!znsock_is_valid(new_socket)) {
    /* accept() error */
    send_code(client, CMD_NETWORK_ERROR);
    return;
  }

  char ip_port[INET6_ADDRSTRLEN + 6];

  /* Convert addr to ip:port */
  if (server_addr_to_str(ip_port, &addr, addr_len)) {
    send_code(client, CMD_INTERNAL_ERROR);
    znsock_close(new_socket, true);
    return;
  }

  /* Define ID with ip_port or new_sock_id if defined */
  char *new_id = id_arg_defined ? msg.argv[2] : ip_port;

  /* Create a new pair */
  id_socket_pair *new_pair = malloc(sizeof(id_socket_pair) + strlen(new_id) + 1);

  if (new_pair == NULL) {
    /* Out of memory */
    send_code(client, CMD_OUT_OF_MEMORY);
    znsock_close(new_socket, true);
    return;
  }

  smutex_lock(&data->pair_mutex);
  unsigned int index;
  id_socket_pair *old_pair = server_get_pair_unlocked(data, new_id, &index);

  if (old_pair) {
    /* Overwritte old pair. */
    overwritten = true;
    znsock_close(old_pair->socket, true);
    server_remove_pair_unlocked(data, index);
  }
  smutex_unlock(&data->pair_mutex);

  new_pair->id = get_str(new_pair);
  strcpy(new_pair->id, new_id);

  new_pair->socket = new_socket;
  new_pair->ipv6 = addr.ss_family == AF_INET6;

  if (server_add_pair(data, new_pair)) {
    send_code(client, CMD_OUT_OF_MEMORY);
    znsock_close(old_pair->socket, true);
    free(new_pair);
    return;
  }

  send_code(client, overwritten ? CMD_OVERWRITTEN : CMD_SUCCESS);

  /* Send ip:port to client. */
  nms_send(client, ip_port, strlen(ip_port));
}
