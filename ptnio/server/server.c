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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <fllist.h>
#include <sthread.h>
#include <znsock.h>
#include <tiny_assert.h>

#include "server.h"
#include "server_cmd.h"

#include "../args_parser.h"
#include "../message.h"
#include "../pipe_fork.h"

#ifndef WIN32
#include <strings.h>
#else
#define strcasecmp stricmp
#endif

static void server_thread(void *_server_socket);

bool server(socket_args args, int message_fd)
{
  server_data data;
  smutex_new(&data.pair_mutex);
  data.args = &args;
  data.pair_list = NULL;
  data.pair_count = 0;

  data.ipc_socket = znsock_ipc_server(args.id, 5);
  if (!znsock_is_valid(data.ipc_socket)) {
    fputs("ERROR: Unable to create the IPC server.\n", stderr);
    return true;
  }

  /* We consider main thread (currently running) as a server thread. */
  size_t thread_count = args.data.server.thread_count - 1;

  int i = 0;
  while (i < thread_count) {
    sthread thread;
    tiny_assert(sthread_new(&thread, server_thread, &data));
    i++;
  }

  #ifndef NO_FORK
  /* Notify parent that server is ready. */
  pipe_send(message_fd);
  #endif

  /* Consider the main thread as a server thread. */
  server_thread(&data);

  /* Never reached (server_thread never returns) */
  return false;
}

static void server_thread(void *_server_data)
{
  server_data *data = _server_data;

  while (true) {
    znsock c = znsock_ipc_accept(data->ipc_socket);

    if (!znsock_is_valid(c))
      continue;

    socket_message msg;

    if (message_recv(c, &msg)) {
      znsock_ipc_close(c);
      continue;
    }

    if (msg.argc != 0) {
      unsigned int i = 0;
      while (i < server_cmds_count) {
        if (strcasecmp(server_cmds[i].key, msg.argv[0]) == 0) {
          server_cmds[i].cmd(msg, c, data);
          break;
        }

        i++;
      }
    }

    znsock_ipc_close(c);
    message_free(msg);
  }
}
