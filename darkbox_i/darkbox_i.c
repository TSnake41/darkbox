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
  		bool blocking = arg[2] != '_';
  		/* getch/kbhit with a small fix for Windows, do not cover every
  		   keys (like non-ASCII/Extended-ASCII-only ones), but most of them
  		*/
  		char k = blocking || kbhit() ? ((k = getch()) != -32 ? k : getch()) : 0;
      return k;

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
      	 " -h: Return a non-nul positive value if data is available in stdin.\n\n"
         "NOTE: darkbox_i support both '-' and '/' as command prefixes.\n");
    return 0;

  get_mouse:
    printf("%d %d %d\n", x, y, b);
    return 0;
}
