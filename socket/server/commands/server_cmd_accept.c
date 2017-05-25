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

/* Syntax : accept listner_sock_id [new_sock_id]
   Usage : Accept a pending socket connection of listner_sock_id.
   If new_sock_id is not defined, define id to "IP:PORT".
*/
void server_cmd_accept(message_t msg, ipc_socket_t client, server_data_t *data)
{
    if (msg.argc < 2) {
        /* Invalid arguments */
        send_code(client, CMD_INVALID_ARGS);
        return;
    }

    bool id_arg_defined = msg.argc > 2;

    id_socket_pair_t *listner_pair = server_get_pair(msg.argv[1], data, NULL);

    if (listner_pair == NULL) {
        /* No pair */
        send_code(client, CMD_NOT_FOUND);
        return;
    }

    struct sockaddr_storage addr = { 0 };
    socklen_t addr_len = sizeof(addr);

    socket_t new_socket = accept(listner_pair->socket, (struct sockaddr *)&addr, &addr_len);

    if (new_socket == -1) {
        /* accept() error */
        send_code(client, CMD_NETWORK_ERROR);
        return;
    }

    #ifdef WIN32
    /* I have some doubts for Windows whether the sure
       socket is blocking or not, so I prefer be
       (for *NIX, this is defined by standards).
    */
    set_blocking(new_socket, true);
    #endif

    char ip_port[INET6_ADDRSTRLEN + 6];

    /* Convert addr to ip:port */
    if (server_addr_to_str(ip_port, &addr, addr_len)) {
        send_code(client, CMD_INTERNAL_ERROR);
        close(new_socket);
        return;
    }

    /* Define ID with ip_port or new_sock_id if defined */
    char *new_id = id_arg_defined ? msg.argv[2] : ip_port;

    /* Create a new pair */
    id_socket_pair_t *new_pair = new_pair(strlen(new_id));

    if (new_pair == NULL) {
        /* Out of memory */
        send_code(client, CMD_OUT_OF_MEMORY);
        close(new_socket);
        return;
    }

    new_pair->socket = new_socket;
    new_pair->ipv6 = addr.ss_family == AF_INET6;

    new_pair->id = get_id(new_pair);
    strcpy(new_pair->id, new_id);

    server_add_pair(data, new_pair);

    send_code(client, CMD_SUCCESS);

    /* Send ip:port to client. */
    nms_send(client, ip_port, strlen(ip_port));
}
