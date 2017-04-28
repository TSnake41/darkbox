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

#ifndef H_SERVER_CMD_UTILS
#define H_SERVER_CMD_UTILS

#include <stdbool.h>
#include <stdlib.h>
#include <socket.h>

#include "../server.h"

#include "../../cmd_codes.h"
#include "../../message.h"

id_socket_pair_t *server_get_pair(char *id, server_data_t *data, unsigned int *index);
void server_add_pair(server_data_t *data, id_socket_pair_t *pair);
void server_remove_pair(server_data_t *data, unsigned int index);

bool server_make_sockaddr(const char *ip, in_port_t port, bool ipv6, struct sockaddr **addr, socklen_t *len);
bool server_get_sockaddr_in(const char *ip, struct sockaddr_in *sock_in);
bool server_get_sockaddr_in6(const char *ip, struct sockaddr_in6 *sock_in);

int send_code(socket_t socket, unsigned char code);
bool parse_bool(const char *str);

#define new_pair(id_len) (calloc(sizeof(id_socket_pair_t) + (id_len) + 1, 1))
#define get_id(pair) (((char *)pair) + sizeof(id_socket_pair_t))

#endif /* H_SERVER_CMD_UTILS */
