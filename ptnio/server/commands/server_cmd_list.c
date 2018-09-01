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
#include <string.h>
#include <nms.h>

#include "server_cmd_utils.h"

/* Syntax : list
   Usage : Get socket list.
*/
void server_cmd_list(socket_message msg, znsock client, server_data *data)
{
  send_code(client, CMD_SUCCESS);

  smutex_lock(&data->pair_mutex);
  for (size_t i = 0; i < data->pair_count; i++) {
    id_socket_pair *pair = data->pair_list[i];
    /* Send id using nms. */
    char new_str[strlen(pair->id) + 2];
    sprintf(new_str, "%s\n", pair->id);
    nms_send(client, new_str, strlen(new_str));
  }
  smutex_unlock(&data->pair_mutex);

  nms_send(client, NULL, 0);
}
