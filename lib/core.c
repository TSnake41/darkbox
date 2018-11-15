/*
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

/* Darkbox console output core library */

#if !defined(_XOPEN_SOURCE) && !defined(WIN32)
#define _XOPEN_SOURCE 700
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "core.h"

#ifdef WIN32
#include <windows.h>

static HANDLE stdout_handle;
static CONSOLE_SCREEN_BUFFER_INFO csbi;
#else
#include <unistd.h>
#include <time.h>

const char Ansi_Table[8] = {
  0, 4, 2, 6,
  1, 5, 3, 7,
};

#endif

#if defined(WIN32) || defined(__DJGPP__)
#include <conio.h>
#endif

#ifdef __DJGPP__
#include <dos.h>
#endif

void core_init(void)
{
  #ifdef WIN32
  stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(stdout_handle, &csbi);
  #endif
}

void core_gotoxy(int x, int y)
{
  if (x < 0 || y < 0)
    return;

  #ifndef WIN32
  printf("\033[%d;%df", y + 1, x + 1);
  #else
  COORD c = { x, y };
  SetConsoleCursorPosition(stdout_handle, c);
  #endif
}

void core_gotoxy_relative(int x, int y)
{
  #ifndef WIN32
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
  #else
  GetConsoleScreenBufferInfo(stdout_handle, &csbi);

  core_gotoxy(
    max(0, csbi.dwCursorPosition.X + x),
    max(0, csbi.dwCursorPosition.Y + y)
  );
  #endif
}

void core_change_cursor_state(bool state)
{
  #ifndef WIN32
  fputs (
    state ?
    "\033[?25h" : /* hide */
    "\033[?25l",  /* show */
    stdout
  );
  #else
  CONSOLE_CURSOR_INFO cursor;

  if (!GetConsoleCursorInfo(stdout_handle, &cursor))
    return;

  cursor.bVisible = state;
  SetConsoleCursorInfo(stdout_handle, &cursor);
  #endif
}


void core_clear_console(void)
{
    #ifndef WIN32
    fputs("\033[2J", stdout);
    #else
    /* Based on libDos9's, Dos9_ClearConsoleScreen function
       Copyright (C) 2010-2016 Romain Garbi
    */

    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten, dwConSize;

    GetConsoleScreenBufferInfo( stdout_handle, &csbi );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter( stdout_handle, (TCHAR) ' ',
      dwConSize, coordScreen, &cCharsWritten );

    GetConsoleScreenBufferInfo( stdout_handle, &csbi );
    FillConsoleOutputAttribute( stdout_handle, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
    #endif

    core_gotoxy(0, 0);
}

void core_change_color(uint8_t color)
{
  #ifndef WIN32
  uint8_t bc = color & 0xF;
  uint8_t fc = color >> 4;

  printf("\033[0m\033[%s3%dm\033[%s4%dm",
    bc > 7 ? "1;" : "", Ansi_Table[bc % 8],
    fc > 7 ? "1;" : "", Ansi_Table[fc % 8]
  );
  #else
  SetConsoleTextAttribute(stdout_handle, color);
  #endif
}

void core_reset_color(void)
{
  #ifndef WIN32
  fputs("\033[39m\033[49m", stdout);
  #else
  SetConsoleTextAttribute(stdout_handle, csbi.wAttributes);
  #endif
}

void core_swritecolor(uint8_t color, const char *s)
{
  if (!s)
    return;

  core_change_color(color);
  fputs(s, stdout);

  core_reset_color();
}

void core_cwritecolor(uint8_t color, int c)
{
  core_change_color(color);
  putchar(c);

  core_reset_color();
}

#ifndef WIN32
void core_sleep(unsigned long ms)
{
  #ifdef __DJGPP__
  delay(ms);
  #else
  struct timespec req;
  time_t sec = (int)(ms / 1000);
  ms -= sec * 1000;
  req.tv_sec = sec;
  req.tv_nsec = ms * 1e+6L;
  while (nanosleep(&req, &req) == -1)
    continue;
  #endif
}
#endif
