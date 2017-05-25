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
#include "server_cmd_utils.h"

/* Syntax : new sock_id [IPv6]
   IPv6 : Use IPv6 instead of IPv4.

   Usage : Create a new socket sock_id.
*/
void server_cmd_new(message_t msg, ipc_socket_t client, server_data_t *data)
{
    /* TODO: Send ip:port to client. */
    if (msg.argc < 2) {
        /* Invalid arguments */
        send_code(client, CMD_INVALID_ARGS);
        return;
    }

    bool overwritten = false;
    unsigned int old_node_index = 0;

    id_socket_pair_t *old_pair = server_get_pair(msg.argv[1], data, &old_node_index);

    /* Remove previous bound socket. */
    if (old_pair != NULL) {
        close(old_pair->socket);
        free(old_pair);

        server_remove_pair(data, old_node_index);

        overwritten = true;
    }

    bool use_ipv6 = msg.argc > 2 && parse_bool(msg.argv[2]);
    int af = use_ipv6 ? AF_INET6 : AF_INET;

    socket_t new_sock = socket(af, SOCK_STREAM, 0);

    if (new_sock == -1) {
        /* Unable to create socket. */
        send_code(client, CMD_NETWORK_ERROR);
        return;
    }

    #ifdef WIN32
    /* See server_cmd_accept for more infos */
    set_blocking(new_sock, true);
    #endif

    /* Add the new socket to the list */

    /* pair : Pair struct + ID string block */
    id_socket_pair_t *pair = new_pair(strlen(msg.argv[1]));
    if (pair == NULL) {
        /* Out of memory */
        send_code(client, CMD_OUT_OF_MEMORY);
        close(new_sock);
        return;
    }

    pair->socket = new_sock;
    pair->id = get_id(pair);
    pair->ipv6 = use_ipv6;
    strcpy(pair->id, msg.argv[1]);

    server_add_pair(data, pair);

    send_code(client, overwritten ? CMD_OVERWRITTEN : CMD_SUCCESS);
}
