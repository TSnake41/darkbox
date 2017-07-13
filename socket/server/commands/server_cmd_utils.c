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
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "server_cmd_utils.h"

id_socket_pair *server_get_pair(char *id, server_data *data, unsigned int *index)
{
    if (index)
        *index = 0;

    smutex_lock(&data->sock_list_mutex);
    fllist *l = data->sock_list;

    while (l) {
        id_socket_pair *pair = l->value;
        if (strcmp(id, pair->id) == 0) {
            smutex_unlock(&data->sock_list_mutex);
            return pair;
        }

        if (index)
            (*index)++;

        l = l->next;
    }
    smutex_unlock(&data->sock_list_mutex);

    return false;
}

void server_add_pair(server_data *data, id_socket_pair *pair)
{
    smutex_lock(&data->sock_list_mutex);
    data->sock_list = list_insert_begin(data->sock_list, pair);
    smutex_unlock(&data->sock_list_mutex);
}

void server_remove_pair(server_data *data, unsigned int index)
{
    smutex_lock(&data->sock_list_mutex);
    data->sock_list = list_remove_at(data->sock_list, index);
    smutex_unlock(&data->sock_list_mutex);
}

int send_code(socket_int socket, uint8_t code)
{
    return send(socket, &code, 1, socket_default_flags);
}

bool parse_bool(const char *str)
{
    return *str == '1' || tolower(*str) == 't';
}

bool server_make_sockaddr(const char *ip, in_port_t port, bool ipv6,
    struct sockaddr **addr, socklen_t *len)
{
    if (ipv6) {
        /* Create sockaddr_in6 */
        struct sockaddr_in6 *sin6 = calloc(sizeof(struct sockaddr_in6), 1);
        *addr = (void *)sin6;

        *len = sizeof(struct sockaddr_in6);

        if (server_get_sockaddr_in6(ip, sin6)) {
            free(sin6);
            return true;
        }

        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = port;
    } else {
        /* Create sockaddr_in */
        struct sockaddr_in *sin = calloc(sizeof(struct sockaddr_in), 1);
        *addr = (void *)sin;

        *len = sizeof(struct sockaddr_in);

        if (server_get_sockaddr_in(ip, sin)) {
            free(sin);
            return true;
        }

        sin->sin_family = AF_INET;
        sin->sin_port = port;
    }

    return false;
}

bool server_get_sockaddr_in(const char *ip, struct sockaddr_in *sock_in)
{
    memset(sock_in, '\0', sizeof(struct sockaddr_in));
    return inet_pton(AF_INET, ip, &sock_in->sin_addr) != 1;
}

bool server_get_sockaddr_in6(const char *ip, struct sockaddr_in6 *sock_in)
{
    memset(sock_in, '\0', sizeof(struct sockaddr_in6));
    return inet_pton(AF_INET6, ip, &sock_in->sin6_addr) != 1;
}

bool server_addr_to_str(char *output, struct sockaddr_storage *addr,
    socklen_t addr_len)
{
    bool is_ipv6 = addr->ss_family == AF_INET6;
    const void *in_addr;

    if (is_ipv6) {
        struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)addr;
        in_addr = &(sin6->sin6_addr);
    } else {
        struct sockaddr_in *sin = (struct sockaddr_in *)addr;
        in_addr = &(sin->sin_addr);
    }

    if (inet_ntop(addr->ss_family, (char *)in_addr, output, addr_len) == NULL)
        /* inet_ntop() failed */
        return true;

    in_port_t port = is_ipv6
        ? ((struct sockaddr_in6 *)addr)->sin6_port
        : ((struct sockaddr_in *)addr)->sin_port;

    port = ntohs(port);

    char port_str[7];
    snprintf(port_str, sizeof(port_str), ":%hu", port);
    strcat(output, port_str);

    return false;
}
