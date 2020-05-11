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
#include <znsock.h>

#include <tiny_assert.h>

#include "client/client.h"
#include "server/server.h"
#include "args_parser.h"

#include "pipe_fork.h"

int main(int argc, char **argv)
{
  if (znsock_init()) {
    perror("znsock_init() failed");
    return -1;
  }

  /* Parse args */
  socket_args args;
  if (parse_args(argv, argc, &args))
    goto show_help;

  bool status;

  if (args.new_instance)
    status = server(args, pipe_fork(argc, argv));
  else
    status = client(args);

  free_args(args);
  znsock_cleanup();
  return status;

  show_help:
    free_args(args);
    puts("ptnio - Portable TCP Network IO - Astie Teddy (TSnake41)\n"
         "Syntaxes :\n"
         "  1: ptnio -id:ID -n [-t thread_count]\n"
         "  2: ptnio -id:ID -c command [command args]\n\n"
         "1: Start a new ptnio instance.\n"
         "2: Connect to a ptnio instance to perform a command.\n\n"
         "For more informations, see README at :\n"
         "https://gitlab.com/TSnake41/darkbox/tree/master/ptnio/README\n");
    return 0;
}
