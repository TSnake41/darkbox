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

#include <core.h>
#include <core_i.h>

#include "darkbox.h"

static bool parse_cmd(darkbox_cmd *command);
static void execute_cmd(darkbox_cmd cmd);

static void input_server(int mode);

static int read_int(void);
static void read_string(char *buffer, const size_t max_length);

#define ENABLE_KEYBOARD 1
#define ENABLE_MOUSE 2
#define KNM_MODE (ENABLE_KEYBOARD | ENABLE_MOUSE)

char text_buffer[MAX_TEXT_LENGTH],
     command_buffer[MAX_COMMAND_SIZE];

int origin_x, origin_y;

int main(int argc, char const *argv[])
{
  if (argc > 1 && (argv[1][0] == '-' || argv[1][0] == '/'))
    switch (argv[1][1]) {
      /* Get help */
      case '?':
        goto show_help;

      /* Start input server */
      case 'i': ;
        #define m_arg (tolower(argv[1][2]))

        int mode = m_arg == 'k' ? ENABLE_KEYBOARD : /* Keyboard mode */
                   m_arg == 'm' ? ENABLE_MOUSE : /* Mouse mode */
                   KNM_MODE; /* Keyboard and Mouse mode */

        #undef m_arg

        input_server(mode);
        return 0;

      /* Returns a non-zero value if data is
         available in stdin otherwise, return 0
      */
      case 'k':
        return core_peek_stdin();
        break;

      case 'w': ;
        if (argc < 2)
          return 1;

        core_sleep(strtol(argv[2], NULL, 10));
        return 0;

        case 'e':
          while (true) {
            fputs("-d \"", stdout);

            /* Use text length to fit in text buffer. */
            for (unsigned long i = 0; i < MAX_TEXT_LENGTH; i++) {
              int c;

              if ((c = getchar()) == EOF)
                /* no more characters */
                break;

              if (c == '\\' || c == '"')
                /* Escape '\' and '"' */
                putchar('\\');

              putchar(c);
            }
            fputs("\" ", stdout);
          }
          break;

    }

  /* Initialize Console IO */
  core_init();

  /* Main loop */
  while (true) {
  	darkbox_cmd cmd;

  	if (parse_cmd(&cmd))
  		execute_cmd(cmd);
  }

  /* Never reached */
  return 0;

  show_help:
    puts("darkbox - Fast Portable Console IO Server - Astie Teddy (TSnake41)\n"
         "Syntaxes :\n"
         "  1: (code) | darkbox\n"
         "  2: darkbox -i[k/m] | (code)\n"
         "  3: darkbox -w t\n"
         "  4: darkbox -k\n\n"
         " 1: Start darkbox as output server\n"
         " 2: Start darkbox as input server\n"
         "    k: Keyboard-only, m: Mouse-only\n"
         " 3: Wait t ms\n"
         " 4: Return a non-nul value if data is avaialble in stdin.\n\n"
         "NOTE: darkbox support both '-' and '/' as command prefixes.\n"
         "For more informations, see README at:\n"
         "https://gitlab.com/TSnake41/darkbox/blob/master/darkbox/README\n");
    return 0;
}

/* Parse the command from stdin if this is not
   a command, print the current line to stdout
*/
static bool parse_cmd(darkbox_cmd *command)
{
  char *str = command_buffer;

  read_string(str, MAX_COMMAND_SIZE);

  if (*str != COMMAND_START_CHR &&
      *str != COMMAND_START_CHR2) {
    /* No command. */
    fputs(str, stdout);
    putchar(' ');

    if (strchr(str, '\n') != 0)
      /* do not break next line */
      return false;

    int c = 0;
    while (c != '\n' && c != EOF)
      putchar(c = getchar());

    if (c == EOF)
      exit(0);

    return false;
	}

  str++;

  /* use the count if specified in command */
  command->count = isdigit(*str)
      ? strtol(str, &str, 10) : 1;

  if (isalpha(*str)) {
    /* define literals */
    command->cmd = str;

    /* check literals */
    while (*str) {
      if (!isalpha(*str)) {
        fputs("Parsing error: Invalid litterals", stderr);
        return false;
      }
      str++;
    }
  } else {
    fputs("Parsing error: No command specified", stderr);
    return false;
  }

  return true;
}

/* Execute a command, get args from stdin */
static void execute_cmd(darkbox_cmd cmd)
{
  for (unsigned int i = 0; i < cmd.count; i++) {
    char *str = cmd.cmd;

    while (*str) {
      switch (tolower(*str)) {
        case 'a':
          /* Syntax : -a Integer
             Usage : Print the character
             corresponding to the integer
          */

          putchar(read_int());
          break;

        case 'd':
          /* Syntax : -d Text
             Usage : Display Text
          */

          read_string(text_buffer, MAX_TEXT_LENGTH);
          fputs(text_buffer, stdout);
          break;

        case 'n':
          /* Syntax : -n
             Usage : Go to the next line
          */

          putchar('\n');
          break;

        case 'c':
          /* Syntax : -c color
             color is in hexadecimal format.

             Usage : Change the current color
                     0xF0 : Background
                     0x0F : Foreground
          */
          core_change_color(read_int());
          break;

        case 'g':
          /* Syntax : -g x y
             Usage : Move cursor to (x;y)
          */

          core_gotoxy(origin_x + read_int(), origin_y + read_int());
          break;

        case 'h':
          /* Syntax : -h 0/1
             Usage : Hide or show cursor
          */

          core_change_cursor_state(read_int());
          break;

        case 's':
          /* Syntax : -s
             Usage : Clear console
          */

          core_clear_console();
          break;

        case 'o':
          /* Syntax : -o x y
             Usage : Change goto origin
          */

          origin_x = read_int();
          origin_y = read_int();
          break;

        case 'r':
          /* Syntax : -r
             Usage : Reset foreground and background colors
          */

          core_reset_color();
          break;

        case 'q':
          /* Syntax : -q
             Usage : Properly stop darkbox
          */

          /* free_buffers(); */
          exit(0);
          break;

        case 'w':
          /* Syntax : -w t
             Usage : Wait t miliseconds
          */

          /* Flush stdout (display any "waiting" stuff) */
          fflush(stdout);
          core_sleep(read_int());
          break;

      /* Drawing variables. */
      int x, y, r, w, h, hollow;

        case 'l': ;
          /* Syntax : -l Ax Ay Bx By
             Usage : Draw a line from A to B.
          */
          int Ax = read_int(),
              Ay = read_int(),
              Bx = read_int(),
              By = read_int();

          /* Code based on :
             https://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C
          */
          int dx = abs(Bx-Ax), sx = Ax < Bx ? 1 : -1;
          int dy = abs(By-Ay), sy = Ay < By ? 1 : -1;
          int err = (dx > dy ? dx : -dy) / 2, e2;

          while (true) {
            core_gotoxy(origin_x + Ax, origin_y + Ay);
            putchar(' ');
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
          break;

        case 'b': ;
          /* Syntax : -b x y w h hollow
             Usage : Draw a box.
          */
          x = read_int();
          y = read_int();
          w = read_int();
          h = read_int();

          hollow = read_int();

          for (int ix = x; ix < (x + w); ix++)
            for (int iy = y; iy < (y + h); iy++)
              if (!hollow || (ix == x || ix == (x + w - 1) || iy == y || iy == (y + h - 1))) {
                core_gotoxy(origin_x + ix, origin_y + iy);
                putchar(' ');
              }
          break;

          /* Draw a circle
             Arguments : x y r hollow
          */
          case 'i': ;
            x = read_int();
            y = read_int();
            r = read_int();

            int rx = r,
                ry = 0;

            err = 0;

            #define circle_plot(cx, cy) core_gotoxy((cx) + origin_x, (cy) + origin_y); \
              putchar(' ')

            fprintf(stderr, "%d %d\n", x, y);

            while (rx >= ry) {
              circle_plot(x + rx, y + ry);
              circle_plot(x + ry, y + rx);
              circle_plot(x - ry, y + rx);
              circle_plot(x - rx, y + ry);
              circle_plot(x - rx, y - ry);
              circle_plot(x - ry, y - rx);
              circle_plot(x + ry, y - rx);
              circle_plot(x + rx, y - ry);

              if (err <= 0) {
                ry++;
                err += 2 * ry + 1;
              } else {
                rx--;
                err -= 2 * rx + 1;
              }
            }
            #undef circle_plot
            break;
      }

      str++;
    }
  }
}

/* Start input server */
static void input_server(int mode)
{
  if (mode & ENABLE_MOUSE)
    core_mouse_initialize(true);

  core_input_event e;

  while (!feof(stdin) && !ferror(stdout)) {
    core_input_get_event(&e);

    switch (e.type) {
      case KEY_PRESS:
        if (mode & ENABLE_KEYBOARD)
            /* Mode allow keyboard event. */
            printf("k %d\n", e.event.key_press);
        break;

      case MOUSE:
        printf("m %d %d %d\n", e.event.mouse.x, e.event.mouse.y, e.event.mouse.b);
        break;
    }

    fflush(stdout);
  }

  if (mode & ENABLE_MOUSE)
    core_mouse_terminate(true);
}
/* Read the next integer from stdin */
int read_int(void)
{
  char c;
  int pos = 0;

  /* skip beginning spaces */
  do
      c = getchar();
  while (isspace(c));

  while (!isspace(c) && pos < MAX_TEXT_LENGTH) {
    text_buffer[pos] = c;
    pos++;
    c = getchar();
  }

  text_buffer[pos++] = '\0';

  return strtol(text_buffer, NULL, 0);
}

/* Read the next argument from stdin */
static void read_string(char *buffer, const size_t max_length)
{
  char c;
  int pos = 0;

  /* skip beginning spaces */
  do
    c = getchar();
  while (isspace(c));

  /* quote mode */
  bool quote = c == '"';

  if (quote)
    /* skip quote chracter */
    c = getchar();

  char *end_chars = quote ? "\"" : " \n\r\t";

  while (!strchr(end_chars, c) && (pos < max_length) ) {
    if (c == '\\')
      /* skip escape chracter, the next chracter is not interpreted */
      c = getchar();

    buffer[pos] = c;
    pos++;

    c = getchar();
  }

  buffer[pos++] = '\0';
}
