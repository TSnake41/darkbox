/*
  Darkbox - A Fast and Portable Console IO Server
  Copyright (C) 2016-2018 Teddy ASTIE

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
#include <stdbool.h>
#include <ctype.h>

#include <core_i.h>

int main(int argc, char **argv)
{
  int x, y, b;

  if (argc == 1)
    /* No arguments specified, show help */
    goto showhelp;

  const char *arg = argv[1];

  if (*arg != '-' && *arg != '/')
    /* Invalid argument */
    return 1;

  switch (tolower(arg[1])) {
    /* Show help */
    case '?':
      goto showhelp;

    /* kbhit */
    case 'h':
      return core_kbhit();

    /* get mouse clic or any non-movement event */
    case 'm':
      core_get_mouse(false, &x, &y, &b);
      goto get_mouse;

    /* get any mouse event including movement */
    case 'y':
      core_get_mouse(true, &x, &y, &b);
      goto get_mouse;

    /* get keyboard key */
    case 'k': ;
      return (arg[2] != '_') || kbhit() ? core_getkey() : 0;

    /* Get either mouse click or keyboard key. */
    case 'z':
      core_mouse_initialize(true);
      
      core_input_event e;
      core_input_get_event(&e);

      switch (e.type) {
        case CORE_EVENT_KEYBOARD:
          printf("k %d\n", e.event.key_press);
          break;

        case CORE_EVENT_MOUSE:
          printf("m %d %d %d\n", e.event.mouse.x, e.event.mouse.y, e.event.mouse.b);
          break;
      }
      core_mouse_terminate(true);
      break;

    default:
      goto showhelp;
  }

  /* this part is never reached */
  return 0;

  showhelp:
    puts("darkbox_i - Standalone input command based on core_i - Astie Teddy (TSnake41)\n"
         "Syntax:\n"
         " darkbox_i [-k[_]] OR [-m] OR [-y] OR [-h]\n\n"
         " -k: Return keyboard key to errorlevel\n"
         " -m: Return mouse clic to stdout\n"
         " -y: Return any mouse movement to stdout\n"
         " -h: Return a non-nul positive value if data is available in stdin.\n"
         " -z: Return any input event to stdout.\n\n"
         "NOTE: darkbox_i support both '-' and '/' as command prefixes.\n");
    return 0;

  get_mouse:
    printf("%d %d %d\n", x, y, b);
    return 0;
}
