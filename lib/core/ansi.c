/*
  Copyright (C) 2019 Teddy ASTIE

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
#include <stdint.h>

#ifdef CORE_W10_ANSI
  #include <windows.h>

  #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
  #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x4
  #endif
#endif

#if defined(__DJGPP__) || defined(WIN32)
#include <conio.h>
#endif

#ifdef __DJGPP__
#include <dos.h>
#endif

const char Ansi_Table[8] = {
  0, 4, 2, 6,
  1, 5, 3, 7,
};

void core_init(void)
{
  #ifdef CORE_W10_ANSI
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode;
  
  GetConsoleMode(hOut, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, mode);
  #endif
}

void core_gotoxy(int x, int y)
{
  if (x < 0 || y < 0)
    return;
  
  printf("\033[%d;%df", y + 1, x + 1);
}

void core_gotoxy_relative(int x, int y)
{
  if (x != 0)
    printf("\033[%d%c",
      abs(x),
      x > 0 ? 'C' : 'D'
    );

  if (y != 0)
    printf("\033[%d%c",
      abs(y),
      y > 0 ? 'B' : 'A'
    );
}

void core_change_cursor_state(bool state)
{
  fputs (
    state ?
    "\033[?25h" : /* hide */
    "\033[?25l",  /* show */
    stdout
  );
}


void core_clear_console(void)
{
  fputs("\033[2J", stdout);
  core_gotoxy(0, 0);
}

void core_change_color(uint8_t color)
{
  uint8_t bc = color & 0xF;
  uint8_t fc = color >> 4;

  printf("\033[0m\033[%s3%dm\033[%s4%dm",
    bc > 7 ? "1;" : "", Ansi_Table[bc % 8],
    fc > 7 ? "1;" : "", Ansi_Table[fc % 8]
  );
}

void core_reset_color(void)
{
  fputs("\033[39m\033[49m", stdout);
}