/*
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
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "strcasechr.h"

#include "choice.h"
#include "parse_args.h"

#define s_strchr(str, c, sensitive) \
  (sensitive ? strchr(str, c) : strcasechr(str, c))

static char **get_value(char **argv, char **value);

bool parse_args(int argc, char **argv, choice_args *args)
{
  args->keys = DEFAULT_KEYS;
  args->prompt = NULL;

  args->default_key = 0;
  args->timeout = 0;

  args->display_choices = true;
  args->case_sensitive = false;

  args->display_help = false;

  bool timeout_defined = false;

  while (*(++argv)) {
    char *current_arg = *argv;

    if (*current_arg != '/' && *current_arg != '-') {
      /* Not valid,*/
      fputs(ERROR_UNEXPECTED_ARG, stderr);
      fputs(current_arg, stderr);
      fputc('\n', stderr);
      return true;
    }

    switch (tolower(current_arg[1])) {
      case 'c':
        if (tolower(current_arg[2]) == 's') {
            args->case_sensitive = true;
            break;
        }

        argv = get_value(argv, &args->keys);
        break;

      case 't': ;
        char *n = "";
        argv = get_value(argv, &n);

        char *t_end_ptr;
        args->timeout = strtoul(n, &t_end_ptr, 0);

        if (t_end_ptr == n) {
          fputs(ERROR_UNEXPECTED_VALUE, stderr);
          fputs(n, stderr);
          fputc('\n', stderr);
          return true;
        }
        timeout_defined = true;
        break;

      case 'd': ;
        char *d_keys = "";
        argv = get_value(argv, &d_keys);

        if (*d_keys)
          /* Assumes there is only one key. */
          args->default_key = *d_keys;
        break;

      case 'm':
        argv = get_value(argv, &args->prompt);
        break;

      case 'n':
        args->display_choices = false;
        break;

      case '?':
        args->display_help = true;
        return false;
    }
  }

  /* Late error checking */
  if (args->keys[0] == '\0') {
    fputs(ERROR_NO_KEY, stderr);
    return true;
  }

  if (args->default_key != 0
    && s_strchr(args->keys, args->default_key, args->case_sensitive) == NULL) {

    fputs(ERROR_DEFAULT_KEY_NOT_FOUND, stderr);
    return true;
  }

  if (timeout_defined && args->default_key == 0) {
    fputs(ERROR_DEFAULT_KEY_MISSING, stderr);
    return true;
  } else if (!timeout_defined && args->default_key != 0) {
    fputs(ERROR_TIMEOUT_MISSING, stderr);
    return true;
  }

  return false;
}

static char **get_value(char **argv, char **value)
{
  char *separator = strchr(*argv, ':');

  /* Check if value exists. */
  if (separator != NULL || argv[1] != NULL)
    *value = separator == NULL ? *(++argv) : separator + 1;

  return argv;
}
