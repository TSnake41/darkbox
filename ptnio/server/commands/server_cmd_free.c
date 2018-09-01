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

#include "server_cmd_utils.h"

/* Syntax : free sock_id
   Usage : Close and free sock_id.
*/
void server_cmd_free(socket_message msg, znsock client, server_data *data)
{
  if (msg.argc < 2) {
    /* Invalid arguments */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  /* Lock pair list. */
  smutex_lock(&data->pair_mutex);

  unsigned int index;
  id_socket_pair *pair = server_get_pair_unlocked(data, msg.argv[1], &index);

  if (pair == NULL) {
    /* No pair found */
    smutex_unlock(&data->pair_mutex);
    send_code(client, CMD_NOT_FOUND);
    return;
  }

  znsock socket = pair->socket;

  server_remove_pair_unlocked(data, index);

  /* We can unlock mutex since pair no longer exists in list. */
  smutex_unlock(&data->pair_mutex);

  znsock_close(socket, true);
  send_code(client, CMD_SUCCESS);
}
