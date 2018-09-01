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
#include <znsock.h>
#include <nms.h>

#include "server_cmd_utils.h"

#include <errno.h>

/* Syntax : recv sock_id [<blocking> <count>]

  Usage : Recieve data from socket.
    If arguments are specified :
      - Read <count> bytes in blocking or non-blocking modes.
    Otherwise :
      - Read as much as possible bytes.
*/
void server_cmd_recv(socket_message msg, znsock client, server_data *data)
{
  #define reset_socket(s) znsock_set_blocking((s), true)

  if (msg.argc != 2 && msg.argc < 4) {
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

  int received = 0;
  void *buffer = malloc(0xFFFF);

  if (buffer == NULL) {
    /* Out of memory. */
    send_code(client, CMD_OUT_OF_MEMORY);
    return;
  }

  send_code(client, CMD_SUCCESS);

  if (msg.argc == 2) {
    /* Read as much as possible bytes. */

    /* Set as non-blocking. */
    znsock_set_blocking(pair->socket, false);

    do {
      received = znsock_recv_block(pair->socket, buffer, 0xFFFF, false);
      if (received == -1)
        break;

      if (nms_send(client, buffer, received))
        break;

    } while (received != 0);
  } else { /* msg.argc == 4 */
    /* Read <count> bytes in blocking or non-blocking modes. */
    bool blocking = parse_bool(msg.argv[2]);
    size_t count = strtoul(msg.argv[3], NULL, 0);

    if (!blocking)
      /* Set as non-blocking. */
      znsock_set_blocking(pair->socket, false);

    unsigned int recv_count = count / 0xFFFF,
                 remaining = count - (0xFFFF * recv_count);

    while (recv_count-- || received == 0xFFFF) {
      received = znsock_recv_block(pair->socket, buffer, 0xFFFF, blocking);

      if (received == -1)
        break;

      if (nms_send(client, buffer, received))
        goto error;
    }

    received = znsock_recv_block(pair->socket, buffer, remaining, blocking);

    if (received != -1 && nms_send(client, buffer, received))
      goto error;

    if (received != 0)
      /* Send a 0-size packet. */
      nms_send(client, buffer, 0);
  }

  error:
  reset_socket(pair->socket);
  free(buffer);
}

/* Syntax : send sock_id
   Usage : Send bytes to socket.
*/
void server_cmd_send(socket_message msg, znsock client, server_data *data)
{
  /* NOTE: This code is heavily based on nms_send (might merge ?). */

  if (msg.argc < 1) {
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

  uint16_t received;
  void *buffer = malloc(0xFFFF);

  if (buffer == NULL) {
    /* Out of memory. */
    send_code(client, CMD_OUT_OF_MEMORY);
    return;
  }

  send_code(client, CMD_SUCCESS);

  do {
    if (nms_recv_no_alloc(client, buffer, &received)) {
      /* Broken IPC pipe ? */
      free(buffer);
      return;
    }

    /* Send data to socket */
    if (znsock_send(pair->socket, buffer, received) == -1) {
      /* Unable to send data to socket. */
      send_code(client, CMD_NETWORK_ERROR);
      free(buffer);
      return;
    }

    /* Data sent */
    send_code(client, CMD_SUCCESS);
  } while (received != 0);

  free(buffer);
}
