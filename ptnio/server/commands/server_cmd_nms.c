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
#include <nms.h>

#include "server_cmd_utils.h"

/* Syntax : nms_recv sock_id
   Usage : Recieve data from socket using NMS.
*/
void server_cmd_nms_recv(socket_message msg, znsock client, server_data *data)
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

  #if 0
  if (msg.argc < 2) {
    /* Set timeout */
    long timeout
    if (socket_set_read_timeout(pair->socket, ))
  }
  #endif

  /* Create buffer of 2^16-1 bytes. */
  uint16_t received;
  void *buffer;

  if (nms_recv(pair->socket, &buffer, &received))
    send_code(client, CMD_NETWORK_ERROR);
  else {
    send_code(client, CMD_SUCCESS);

    /* Send back recieved data. */
    nms_send(client, buffer, received);

    if (received > 0)
      free(buffer);
  }
}

/* Syntax : nms_send sock_id
   Usage : Send bytes to socket using NMS.
*/
void server_cmd_nms_send(socket_message msg, znsock client, server_data *data)
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

  uint16_t received;
  void *buffer = malloc(0xFFFF);

  if (buffer == NULL) {
    /* Out of memory. */
    send_code(client, CMD_OUT_OF_MEMORY);
    return;
  }

  /* TODO: Everything is okay but needs some cleanup. */

  send_code(client, CMD_SUCCESS);

  if (nms_recv_no_alloc(client, buffer, &received)) {
    /* Assumes that the IPC socket pipe is broken.
       So, do not send any message because it will *probably* fail.
    */
    free(buffer);
    return;
  }

  do {
    /* Send data to socket */
    if (nms_send(pair->socket, buffer, received)) {
      /* Unable to send data to client. */
      send_code(client, CMD_NETWORK_ERROR);
      break;
    }

    /* Data sent */
    send_code(client, CMD_SUCCESS);

    if (nms_recv_no_alloc(client, buffer, &received))
      break;

  } while (received != 0);

  send_code(client, CMD_SUCCESS);
  free(buffer);
}
