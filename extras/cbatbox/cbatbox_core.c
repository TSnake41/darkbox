/*
   Copyright (C) 2017-2018 ASTIE Teddy (TSnake41)

   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
   OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include <core.h>
#include <core_i.h>

int main(int argc, char **argv)
{
  core_init();

  int ox = 0, oy = 0;
  argc--;
  argv++;

  for (int i = 0; i < argc; i++)
    switch(tolower(argv[i][1])) {
      case 'a': {
        if (argc - i < 1)
          return 0;

        putchar(strtol(argv[i + 1], NULL, 0));
        i++;
        break;
      }

      case 'g': {
        if (argc - i < 2)
          return 0;

        core_gotoxy (
          ox + strtol(argv[i + 1], NULL, 0),
          oy + strtol(argv[i + 2], NULL, 0)
        );

        i += 2;
        break;
      }

      case 'c': {
        if (argc - i < 1)
          return 0;

        core_change_color(strtol(argv[i + 1], NULL, 0));
        i++;
        break;
      }

      case 'd': {
        if (argc - i < 1)
          return 0;

        fputs(argv[i + 1], stdout);
        i++;
        break;
      }

      case 'o': {
        if (argc - i < 2)
          return 0;

        ox = strtol(argv[i + 1], NULL, 0);
        oy = strtol(argv[i + 2], NULL, 0);

        i += 2;
        break;
      }

      case 'w': {
        if (argc - i < 1)
          return 0;

        core_sleep(strtol(argv[i + 1], NULL, 0));
        i++;
        break;
      }

      case 'h': {
        if (argc - i < 1)
          return 0;

        core_change_cursor_state(strtol(argv[i + 1], NULL, 0));
        i++;
        break;
      }

      case 'm': {
        int x, y, b;
        core_get_mouse(false, &x, &y, &b);
        printf("%d:%d:%d", x, y, b);
        break;
      }

      case 'y': {
        int x, y, b;
        core_get_mouse(true, &x, &y, &b);
        printf("%d:%d:%d", x, y, b);
        break;
      }

      case 'k': {
        if (argv[i][2] == '_' && !core_kbhit())
          break;
        
        return core_getkey();
      }

      case 'p': {
        /* Ignored */
        if (argc - i < 1)
          return 0;

        i++;
        break;
      }

      case 'n': {
        putchar('\n');
        break;
      }

      case 'x': {
        /* TODO */
      }
    }

  return 0;
}
