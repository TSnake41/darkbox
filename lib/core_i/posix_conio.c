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

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <stdio.h>

#include <sys/ioctl.h>
#include <termios.h>

int getch(void)
{
  struct termios oldattr, newattr;
  int ch;
  tcgetattr(fileno(stdin), &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~( ICANON | ECHO );
  tcsetattr(fileno(stdin), TCSANOW, &newattr);
  ch = getchar();
  tcsetattr(fileno(stdin), TCSANOW, &oldattr);
  return ch;
}

/* Morgan McGuire, morgan@cs.brown.edu */
int kbhit(void)
{
  static int nbf = 0;

  if (!nbf) {
    setvbuf(stdin, NULL, _IONBF, 0);
    nbf = 1;
  }

  int bytesWaiting;
  ioctl(fileno(stdin), FIONREAD, &bytesWaiting);
  return bytesWaiting;
}
