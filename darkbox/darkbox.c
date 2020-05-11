/*
  Darkbox - A Fast and Portable Console IO Server
  Copyright (C) 2016-2019 Teddy ASTIE

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

static const char *mouse_format = "%d %d %d\n";

int main(int argc, char const *argv[])
{
  if (argc > 1 && (argv[1][0] == '-' || argv[1][0] == '/'))
    switch (argv[1][1]) {
      /* Get help */
      case '?':
        goto show_help;

      /* Start input server */
      case 'i': ;
        int m_arg = tolower(argv[1][2]);

        int mode =
          m_arg == 'k' ? ENABLE_KEYBOARD : /* Keyboard mode */
          m_arg == 'm' ? ENABLE_MOUSE : /* Mouse mode */
          KNM_MODE; /* Keyboard and Mouse mode */

        input_server(mode);
        return 0;

      /* Returns a non-zero value if data is
         available in stdin otherwise, return 0
      */
      case 'h':
        return core_peek_stdin();
        break;

      /* Get mouse clic or any non-movement event */
      case 'm': {
        int x, y, b;
        core_get_mouse(false, &x, &y, &b);
        printf(mouse_format, x, y, b);
        return 0;
      }

      /* Get any mouse event including movement */
      case 'y': {
        int x, y, b;
        core_get_mouse(true, &x, &y, &b);
        printf(mouse_format, x, y, b);
        return 0;
      }

      /* Get a keyboard key. */
      case 'k': ;
        return (argv[1][2] != '_') || kbhit() ? core_getkey() : 0;

      /* Wait a specified time (in ms). */
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

              if ((c = getchar()) == EOF) {
                /* no more characters */
                /* Terminate string */
                putchar('"');
                return 0;
              }

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
         "  4: darkbox -h\n"
         "  5: darkbox -k[_]\n"
         "  6: darkbox -[m/y]\n\n"
         " 1: Start darkbox as output server\n"
         " 2: Start darkbox as input server\n"
         "    k: Keyboard-only, m: Mouse-only\n"
         " 3: Wait t ms\n"
         " 4: Return a non-nul value if data is avaialble in stdin.\n"
         " 5: Returns the keycode of the next key pressed.\n"
         "    With '_', returns immediatly with 0 if no key is pressed.\n"
         " 6: Wait for a mouse event and print it to stdout.\n"
         "    m: Any non-movement event, y: Any mouse event\n\n"
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

          case 'g': {
            /* Syntax : -g x y
               Usage : Move cursor to (x;y)
            */

            /* Read X first. */
            int gx = origin_x + read_int();
            core_gotoxy(gx, origin_y + read_int());
            break;
          }

          case 'j': {
            /* Syntax : -j x y
               Usage : Move cursor to offset (x;y)
                       from current position.

               NOTE: Ignores origin.
            */

            /* Read X first. */
            int gx = read_int();
            core_gotoxy_relative(gx, read_int());
            break;
          }

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

        case 'l': {
          /* Syntax : -l Ax Ay Bx By
             Usage : Draw a line from A to B.
          */
          int Ax = read_int();
          int Ay = read_int();
          int Bx = read_int();
          int By = read_int();

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
        }

        case 'b': {
          /* Syntax : -b x y w h hollow
             Usage : Draw a box.
          */
          int x = read_int();
          int y = read_int();
          int w = read_int();
          int h = read_int();

          int hollow = read_int();

          for (int ix = x; ix < (x + w); ix++)
            for (int iy = y; iy < (y + h); iy++)
              if (!hollow || (ix == x || ix == (x + w - 1) || iy == y || iy == (y + h - 1))) {
                core_gotoxy(origin_x + ix, origin_y + iy);
                putchar(' ');
              }

          break;
        }

        case 'i': {
          /* Syntax : -i x y r
             Usage : Draw a circle.
          */
          int x = read_int();
          int y = read_int();
          int r = read_int();

          int rx = r, ry = 0;

          int err = 0;

          #define circle_plot(cx, cy) do { \
            core_gotoxy((cx) + origin_x, (cy) + origin_y); \
            putchar(' '); \
          } while (0)

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
      case CORE_EVENT_KEYBOARD:
        if (mode & ENABLE_KEYBOARD)
            /* Mode allow keyboard event. */
            printf("k %d\n", e.event.key_press);
        break;

      case CORE_EVENT_MOUSE:
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
