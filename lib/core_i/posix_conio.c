/*
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif

#include <stdio.h>
#include <unistd.h>
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

  /* Handle special chracters */
  if (ch == '\033' && getch() == '[')
    switch (getch()) {
      case 'A': /* up arrow */
        return 72;
        break;
      case 'B': /* down arrow */
        return 80;
        break;
      case 'C': /* right arrow */
        return 77;
        break;
      case 'D': /* left arrow */
        return 75;
        break;
      case 'F': /* end */
        return 79;
        break;
      case 'H': /* begin */
        return 71;
        break;

      case '2': /* insert */
        getch(); /* ignore the next character */
        return 82;
        break;
      case '3': /* delete */
        getch();
        return 83;
        break;
      case '5': /* page up */
        getch();
        return 73;
        break;
      case '6': /* page down */
        getch();
        return 81;
        break;

      default:
        return -2; /* unmanaged/unknown key */
        break;
    }

  else
    return ch;
}

/* Morgan McGuire, morgan@cs.brown.edu */
int kbhit(void)
{
  static char initialized = 0;

  if (! initialized) {
    // Use termios to turn off line buffering
    struct termios term;
    tcgetattr(fileno(stdin), &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(fileno(stdin), TCSANOW, &term);
    setbuf(stdin, NULL);
    initialized = 1;
  }

  int bytesWaiting;
  ioctl(fileno(stdin), FIONREAD, &bytesWaiting);
  return bytesWaiting;
}
