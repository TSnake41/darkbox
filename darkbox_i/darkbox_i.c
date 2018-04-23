/*
    Darkbox - A Fast and Portable Console IO Server
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met :

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the name of Teddy Astie (TSnake41) nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY TEDDY ASTIE AND CONTRIBUTORS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL TEDDY ASTIE AND CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
    LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include <core_i.h>

int main(int argc, char **argv)
{
  unsigned int x, y, b;

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
  		   keys (like non-ASCII/Extended-ASCIIonly ones), but most of them
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
