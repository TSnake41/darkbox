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
