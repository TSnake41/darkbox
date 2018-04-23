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
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

#define min(a,b) ( ((a) < (b) ) ? (a) : (b) )
#define max(a,b) ( ((a) > (b) ) ? (a) : (b) )

#define plot(x, y, c) printf("-gcar %d %d 0x%c0 32 ", x, y, c)

#define g_plot_init(count) printf("-%dgcar ", count)
#define g_plot(x, y, c) printf("%d %d 0x%c0 32 ", x, y, c)

int main(int argc, char const *argv[])
{
  if (argc == 1)
    /* No arguments specified, show help. */
    goto show_help;

  const char *arg = argv[1];

  if (*arg != '-' && *arg != '/')
    /* Invalid argument */
    return 1;

  switch (tolower(arg[1])) {
    /* Show help */
    case 'h':
        goto show_help;

    /* Read stdout and convert output to darkbox commands
       Argument : [bs]
    */
    case 'e':
    {
      unsigned long bs = 16;

      /* Read bs if defined */
      if (argc > 2)
        bs = strtoul(argv[2], NULL, 0);

      do {
        fputs("-d \"", stdout);

        for (unsigned long i = 0; i < bs; i++) {
          int c;

          if ((c = getchar()) == EOF)
            break;

          if (c == '\\' || c == '"')
            /* Escape '\' and '"' */
            putchar('\\');

          putchar(c);
        }
        fputs("\" ", stdout);
      } while(!feof(stdin));
    }
    break;

    /* Draw a line
       Arguments : color Ax Ay Bx By
       Uses Bresenham's line algorithm.
    */
    case 'l':
    {
      if (argc < 7)
        return 1;

      char color = *(argv[2]);

      int Ax = strtol(argv[3], NULL, 0),
          Ay = strtol(argv[4], NULL, 0),
          Bx = strtol(argv[5], NULL, 0),
          By = strtol(argv[6], NULL, 0);

      /* Code based on :
         https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
      */
      int dx = abs(Bx-Ax), sx = Ax < Bx ? 1 : -1;
      int dy = abs(By-Ay), sy = Ay < By ? 1 : -1;
      int err = (dx > dy ? dx : -dy) / 2, e2;

      while (true) {
        plot(Ax, Ay, color);
        if (Ax == Bx && Ay == By)
          break;
        e2 = err;
        if (e2 >-dx) {
          err -= dy;
          Ax += sx;
        }

        if (e2 < dy) {
          err += dx;
          Ay += sy;
        }
      }
    }
    break;

    /* Draw a box
       Arguments : color x y w h [-hollow]
    */
    case 'b':
    {
      if (argc < 7)
        return 1;

      char color = *(argv[2]);

      int x = strtol(argv[3], NULL, 0),
          y = strtol(argv[4], NULL, 0),
          w = strtol(argv[5], NULL, 0),
          h = strtol(argv[6], NULL, 0);

      bool hollow = false;

      if (argc > 7 && !strncmp(argv[7], "-h", 2))
        hollow = true;

      g_plot_init (
        hollow ? (w + h - 2) * 2 :
                 (w * h)
      );

      for (int ix = x; ix < (x + w); ix++)
        for (int iy = y; iy < (y + h); iy++)
          if (!hollow || (ix == x || ix == (x + w - 1) || iy == y || iy == (y + h - 1)))
            g_plot(ix, iy, color);
    }
    break;

    /* Draw a circle
       Arguments : color x y r [-hollow]
    */
    case 'c':
    {
      if (argc < 6)
        return 1;

      char color = argv[2][0];

      int x = strtol(argv[3], NULL, 0);
      int y = strtol(argv[4], NULL, 0);
      int r = strtol(argv[5], NULL, 0);

      int rx = r,
          ry = 0,
          err = 0;

      while (rx >= ry) {
        plot(x + rx, y + ry, color);
        plot(x + ry, y + rx, color);
        plot(x - ry, y + rx, color);
        plot(x - rx, y + ry, color);
        plot(x - rx, y - ry, color);
        plot(x - ry, y - rx, color);
        plot(x + ry, y - rx, color);
        plot(x + rx, y - ry, color);

        if (err <= 0) {
          ry++;
          err += 2 * ry + 1;
        } else {
          rx--;
          err -= 2 * rx + 1;
        }
      }
    }
    break;

    default:
      goto show_help;
  }

  return 0;

  show_help:
    puts("darkbox_t - Darkbox toolkit module - Astie Teddy (TSnake41)\n"
         "Syntaxes :\n"
         "  (code) | darkbox_t -e [-bs:size]\n"
         "  darkbox_t -l color Ax Ay Bx By\n"
         "  darkbox_t -b color x y w h [-hollow]\n"
         "  darkbox_t -c color x y r\n\n"
         "NOTE: darkbox_t support both '-' and '/' as command prefixes.\n");
    return 0;
}
