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
#include <nms.h>

#include "server_cmd_utils.h"

/* Syntax : nms_recv sock_id
   Usage : Recieve data from socket using NMS.
*/
void server_cmd_nms_recv(message_t msg, ipc_socket_t client, server_data_t *data)
{
    if (msg.argc != 2) {
        /* Invalid arguments */
        send_code(client, CMD_INVALID_ARGS);
        return;
    }

    id_socket_pair_t *pair = server_get_pair(msg.argv[1], data, NULL);

    if (pair == NULL) {
        /* No pair found */
        send_code(client, CMD_NOT_FOUND);
        return;
    }

    /* Create buffer of 2^16-1 bytes. */
    uint16_t recieved;
    void *buffer;

    if (nms_recv(pair->socket, &buffer, &recieved))
        /* nms_recv failed, send nothing to client. */
        return;

    /* Send back recieved data. */
    nms_send(client, buffer, recieved);

    free(buffer);
}

/* Syntax : nms_send sock_id
   Usage : Send bytes to socket using NMS.
*/
void server_cmd_nms_send(message_t msg, ipc_socket_t client, server_data_t *data)
{
    if (msg.argc != 2) {
        /* Invalid arguments */
        send_code(client, CMD_INVALID_ARGS);
        return;
    }

    id_socket_pair_t *pair = server_get_pair(msg.argv[1], data, NULL);

    if (pair == NULL) {
        /* No pair found */
        send_code(client, CMD_NOT_FOUND);
        return;
    }

    uint16_t recieved;
    void *buffer;

    if (nms_recv(client, &buffer, &recieved))
        /* nms_recv from client failed ??? */
        return;

    /* Send data to socket */
    if (nms_send(client, buffer, recieved)) {
        /* Unable to send data to client. */
        send_code(client, CMD_NETWORK_ERROR);
        free(buffer);
        return;
    }

    send_code(client, CMD_SUCCESS);
}
