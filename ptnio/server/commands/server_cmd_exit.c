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

#include <stdlib.h>
#include <znsock.h>

#include "server_cmd_utils.h"

/* Syntax : exit
   Usage : Ends server.
*/
void server_cmd_exit(socket_message msg, znsock client, server_data *data)
{
  /* Lock mutex (lock other thread). */
  smutex_lock(&data->pair_mutex);

  /* Close IPC socket, thus disabling IPC. */
  znsock_close(close(client), true);
  znsock_close(data->ipc_socket, false);

  /* Force all sockets to close. */
  size_t i = data->pair_count;
  while (i--) {
    znsock_close(data->pair_list[0]->socket, false);
    server_remove_pair_unlocked(data, 0);
  }

  /* Exit current process */
  exit(0);
}
