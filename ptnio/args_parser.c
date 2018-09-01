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
#include <stdbool.h>

#include "args_parser.h"

bool parse_args(char **argv, int argc, socket_args *args)
{
  bool context_defined = false;
  memset(args, '\0', sizeof(socket_args));

  int i = 0;
  while (i < argc) {
    if (argv[i][0] != '-') {
      /* Invalid argument */
      i++;
      continue;
    }

    char *s = argv[i] + 1,
         *v = strchr(s, ':');

    switch (tolower(*s)) {
      case 'i':
        if (v != NULL)
          args->id = v + 1;
        else {
          fputs("ERROR: ID defined without value.\n", stderr);
          return true;
        }
        break;

      case 'n':
        context_defined = true;
        args->new_instance = true;
        args->data.server.thread_count = THREAD_COUNT;
        break;

      case 't':
        if (!args->new_instance) {
          fputs("ERROR: Attempted to define 't' in non-server context.\n", stderr);
          return true;
        }

        if (v != NULL)
            args->data.server.thread_count = strtoul(v + 1, NULL, 0);
        else {
            fputs("ERROR: 't' defined without value.\n", stderr);
            return true;
        }
        break;

      case 'c':
        context_defined = true;
        client_request_data *client_data = &(args->data.client);
        i++;

        /* Begin client command list */
        int c_argc = client_data->command_argc = argc - i;

        if (c_argc < 1) {
          fputs("ERROR: Command expected.\n", stderr);
          return true;
        }

        client_data->command_argv = calloc(c_argc, sizeof(char *));

        if (client_data->command_argv == NULL) {
          fputs("ERROR: Out of memory.\n", stderr);
          return true;
        }

        int j = 0;
        while (j < c_argc) {
          client_data->command_argv[j] = argv[i + j];
          j++;
        }

        goto skip_loop;
    }

    i++;
  }

  skip_loop:
    if (args->id == NULL) {
      fputs("ERROR: ID not defined.\n", stderr);
      return true;
    }

    if (!context_defined) {
      fputs("INFO: No context defined.\n", stderr);
      return true;
    }

    return false;
}

void free_args(socket_args args)
{
  if (!args.new_instance && args.data.client.command_argv != NULL)
    free(args.data.client.command_argv);
}
