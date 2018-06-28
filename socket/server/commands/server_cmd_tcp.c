/*
    SockeT - Portable TCP and NMS Network IO interface.
    Copyright (c) 2017-2018 Teddy ASTIE (TSnake41)

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
#include <socket.h>
#include <nms.h>

#include "server_cmd_utils.h"

/* Min: 1, Max: 0xFFFF */
#define RECV_BUFFER_SIZE 0xFFFF

/* Syntax : recv sock_id [<blocking> <count>]

  Usage : Recieve data from socket.
    If arguments are specified :
      - Read <count> bytes in blocking or non-blocking modes.
    Otherwise :
      - Read as much as possible bytes.
*/
void server_cmd_recv(socket_message msg, socket_int client, server_data *data)
{
  #define reset_socket(s) socket_set_blocking((s), true)

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

  long recieved = 0;
  void *buffer = malloc(RECV_BUFFER_SIZE);

  if (buffer == NULL) {
    /* Out of memory. */
    send_code(client, CMD_OUT_OF_MEMORY);
    return;
  }

  send_code(client, CMD_SUCCESS);

  if (msg.argc == 2) {
    /* Read as much as possible bytes. */

    /* Set as non-blocking. */
    socket_set_blocking(pair->socket, false);

    do {
      recieved = recv(pair->socket, buffer, RECV_BUFFER_SIZE, socket_default_flags);
      if (recieved == -1)
        break;

      if (nms_send(client, buffer, recieved))
        break;

    } while (recieved != 0);

    nms_send(client, buffer, 0);
  } else { /* msg.argc == 4 */
    /* Read <count> bytes in blocking or non-blocking modes. */
    bool blocking = parse_bool(msg.argv[2]);
    size_t count = strtoul(msg.argv[3], NULL, 0);

    if (!blocking)
      /* Set as non-blocking. */
      socket_set_blocking(pair->socket, false);

    unsigned int recv_count = count / RECV_BUFFER_SIZE,
                 remaining = count - (RECV_BUFFER_SIZE * recv_count);

    while (recv_count-- || recieved == RECV_BUFFER_SIZE) {
      recieved = recv(pair->socket, buffer, RECV_BUFFER_SIZE, socket_default_flags);
      if (recieved == -1)
        break;

      if (nms_send(client, buffer, recieved))
        goto error;
    }

    recieved = recv(pair->socket, buffer, remaining, socket_default_flags);

    if (recieved != -1 && nms_send(client, buffer, recieved))
      goto error;

    if (recieved != 0)
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
void server_cmd_send(socket_message msg, socket_int client, server_data *data)
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

  uint16_t recieved;
  void *buffer = malloc(0xFFFF);

  if (buffer == NULL) {
    /* Out of memory. */
    send_code(client, CMD_OUT_OF_MEMORY);
    return;
  }

  do {
    if (nms_recv_no_alloc(client, buffer, &recieved)) {
      /* Broken IPC pipe ? */
      send_code(client, CMD_IPC_ERROR);
      free(buffer);
      return;
    }

    /* Send data to socket */
    if (send(pair->socket, buffer, recieved, socket_default_flags) == -1) {
      /* Unable to send data to socket. */
      send_code(client, CMD_NETWORK_ERROR);
      free(buffer);
      return;
    }
  } while (recieved == 0xFFFF);

  send_code(client, CMD_SUCCESS);
  free(buffer);
}
