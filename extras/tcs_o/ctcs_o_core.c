/*
  Copyright (C) 2018 Teddy ASTIE

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

#include <core.h>

#define STR_BUF_SIZE 1024

char str_buf[1024];
unsigned int ebx; /* HACK: read_str_'s ebx. */
int origin_x = 0, origin_y = 0;

char read_char_(void)
{
  int c = getchar();

  if (c == EOF)
    exit(0);

  return c;
}

void read_str_(void)
{
  int c = 0, str_mode = 0;

  /* Skip all spaces */
  while (isspace(c = read_char_())) {}

  if (c != '"') {
    ebx = 1; /* Offset in str_buf */
    str_mode = 1; /* Reading mode: space */
    str_buf[0] = c; /* NOTE: *str_buf = c; */
  } else {
    /* read_str_quote_ */
    ebx = 0; /* Offset is 0 */
    str_mode = 2; /* Reading mode: quote */
  }

  /* read_str_loop_ */
  while (true) {
    c = read_char_();

    if (c == '\\') {
      /* read_str_escape_ */
      /* Read next char (skip "\)" */
      c = read_char_();
    } else if (str_mode ? isspace(c) : (c == '"')) /* read_str_cmp_ */
      /* read_str_end_ */
      break;

    /* read_str_escape_end_ */
    if (ebx == STR_BUF_SIZE)
      break;

    str_buf[ebx] = c;
    ebx++;
  }

  /* read_str_end_ */
}

long read_number_(void)
{
  read_str_();

  /* Add '\0' to get a Null-terminated string. */
  str_buf[ebx] = '\0';

  return strtol(str_buf, NULL, 0);
}

/* Entry point. */
int main(void)
{
  core_init();
  
  while (true) { /* loop_ */
    if (read_char_() != '-')
      continue;

    /* Get command char */
    switch (read_char_()) {
      case 'a': /* ascii_ */
        putchar(read_number_());
        break;

      case 'c': /* color_ */
        core_change_color(read_number_());
        break;

      case 'd': /* disp_ */
        read_str_();

        fwrite(str_buf, 1, ebx, stdout);
        break;

      case 'n': /* new_line_ */
        putchar('\n');
        break;

      case 's': /* clear_ */
        core_clear_console();
        break;

      case 'g':; /* goto_ */
        int x = origin_x + read_number_();
        int y = origin_y + read_number_();

        core_gotoxy(x, y);
        break;

      case 'o': /* origin_ */
        origin_x = read_number_();
        origin_y = read_number_();
        break;

      case 'w': /* wait_ */
        core_sleep(read_number_());
        break;

      case 'q': /* quit_ */
        exit(0);
        break;
    }
  }
  
  return 0;
}
