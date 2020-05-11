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
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include "server_cmd_utils.h"

id_socket_pair *server_get_pair_unlocked(server_data *data, char *id, unsigned int *index)
{
  for (unsigned int i = 0; i < data->pair_count; i++)
    if (strcmp(id, data->pair_list[i]->id) == 0) {
      if (index)
        *index = i;

      return data->pair_list[i];
    }

  return NULL;
}

id_socket_pair *server_get_pair(server_data *data, char *id, unsigned int *index)
{
  smutex_lock(&data->pair_mutex);
  id_socket_pair *pair = server_get_pair_unlocked(data, id, index);
  smutex_unlock(&data->pair_mutex);
  return pair;
}

bool server_add_pair(server_data *data, id_socket_pair *pair)
{
  smutex_lock(&data->pair_mutex);

  /* Resize pair_list to carry one more id_socket_pair. */
  id_socket_pair **new_pair_list = realloc(data->pair_list,
    sizeof(id_socket_pair *) * ++data->pair_count
  );

  if (new_pair_list == NULL) {
    /* Realloc failed. */
    smutex_unlock(&data->pair_mutex);
    return true;
  }

  /* Add new pair to list. */
  new_pair_list[data->pair_count - 1] = pair;

  data->pair_list = new_pair_list;

  smutex_unlock(&data->pair_mutex);
  return false;
}

void server_remove_pair_unlocked(server_data *data, unsigned int index)
{
  if (index >= data->pair_count)
    /* Out of bounds */
    return;

  /* Free pair */
  free(data->pair_list[index]);

  /* Override old pair with next members (shift all next members to left). */
  memmove (
    &data->pair_list[index],
    &data->pair_list[index + 1],
    (data->pair_count - index - 1) * sizeof(id_socket_pair *)
  );

  id_socket_pair **new_pair_list = realloc(data->pair_list,
    sizeof(id_socket_pair *) * --data->pair_count
  );

  if (new_pair_list != NULL || data->pair_count == 0)
    data->pair_list = new_pair_list;

  /* In case new_pair_list is NULL (the realloc failed), old memory
     still valid, however, only one pair in the allocation unit is surplus
     (which does not affect behavior since data->pair_count determines the
     number of pairs).
  */
}

void server_remove_pair(server_data *data, unsigned int index)
{
  smutex_lock(&data->pair_mutex);
  server_remove_pair_unlocked(data, index);
  smutex_unlock(&data->pair_mutex);
}

int send_code(znsock socket, uint8_t code)
{
  return znsock_send(socket, &code, 1);
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
    if (sin6 == NULL)
      return true;

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
    if (sin == NULL)
      return true;

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
