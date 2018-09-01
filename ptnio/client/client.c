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

#include <znsock.h>
#include <tiny_assert.h>

#include "../args_parser.h"
#include "../message.h"

#include "client_handle.h"

bool client(socket_args args)
{
  znsock socket = znsock_ipc_client(args.id);
  if (!znsock_is_valid(socket)) {
    fputs("ERROR: Unable to connect to IPC server.\n", stderr);
    return true;
  }

  socket_message msg = {
    .argc = args.data.client.command_argc,
    .argv = args.data.client.command_argv
  };

  tiny_assert(message_send(socket, msg));

  /* Execute corresponding callback. */
  unsigned int i = 0;
  while (i < client_handles_count) {
    client_handle handle = client_handles[i];

    if (strcmp(handle.cmd, msg.argv[0]) == 0)
      handle.func(socket);

    i++;
  }

  znsock_ipc_close(socket);
  return false;
}
