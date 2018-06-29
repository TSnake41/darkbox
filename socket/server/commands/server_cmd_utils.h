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

#ifndef H_SERVER_CMD_UTILS
#define H_SERVER_CMD_UTILS

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <znsock.h>

#include "../server.h"

#include "../../cmd_codes.h"
#include "../../message.h"

#define server_default_flags (MSG_NOSIGNAL)

/**
 * Get a pair pointer and index by it's id.
 */
id_socket_pair *server_get_pair(server_data *data, char *id, unsigned int *index);

/**
 * Lock-free variant of server_get_pair().
 */
id_socket_pair *server_get_pair_unlocked(server_data *data, char *id, unsigned int *index);

/**
 * Add a pair in pair list.
 */
bool server_add_pair(server_data *data, id_socket_pair *pair);

/**
 * Remove a pair from pair list by it's index.
 * NOTE: Make sure your index still point to the right pointer.
 */
void server_remove_pair(server_data *data, unsigned int index);

/**
 * Lock-free variant of server_remove_pair().
 */
void server_remove_pair_unlocked(server_data *data, unsigned int index);

/**
 * Initialize a sockaddr struct.
 */
bool server_make_sockaddr(const char *ip, in_port_t port, bool ipv6, struct sockaddr **addr, socklen_t *len);

/**
 * Create a sockaddr_in form an IPv4 IP.
 */
bool server_get_sockaddr_in(const char *ip, struct sockaddr_in *sock_in);

/**
 * Create a sockaddr_in6 form an IPv6 IP.
 */
bool server_get_sockaddr_in6(const char *ip, struct sockaddr_in6 *sock_in);

/**
 * Get IP string form sockaddr.
 * output must be able to store at least (INET6_ADDRSTRLEN + 6) bytes.
 */
bool server_addr_to_str(char *output, struct sockaddr_storage *addr, socklen_t addr_len);

/**
 * Send a single byte (should be used as status reporting with IPC).
 */
int send_code(znsock socket, uint8_t code);

bool parse_bool(const char *str);

#define get_str(pair_ptr) ((char *)(pair_ptr + 1))

#if (!defined(WIN32) || defined(FORCE_POLL)) && !defined(FORCE_SELECT)
#define USE_POLL
#else
#define USE_SELECT
#endif

#endif /* H_SERVER_CMD_UTILS */
