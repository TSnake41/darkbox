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
#include <stdbool.h>
#include <stdint.h>

#include <windows.h>
#include <conio.h>

static HANDLE stdout_handle;
static CONSOLE_SCREEN_BUFFER_INFO csbi;

void core_init(void)
{
  stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(stdout_handle, &csbi);
}

void core_gotoxy(int x, int y)
{
  if (x < 0 || y < 0)
    return;

  COORD c = { x, y };
  SetConsoleCursorPosition(stdout_handle, c);
}

void core_gotoxy_relative(int x, int y)
{
  GetConsoleScreenBufferInfo(stdout_handle, &csbi);

  core_gotoxy (
    max(0, csbi.dwCursorPosition.X + x),
    max(0, csbi.dwCursorPosition.Y + y)
  );
}

void core_change_cursor_state(bool state)
{
  CONSOLE_CURSOR_INFO cursor;

  if (!GetConsoleCursorInfo(stdout_handle, &cursor))
    return;

  cursor.bVisible = state;
  SetConsoleCursorInfo(stdout_handle, &cursor);
}


void core_clear_console(void)
{
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

  core_gotoxy(0, 0);
}

void core_change_color(uint8_t color)
{
  SetConsoleTextAttribute(stdout_handle, color);
}

void core_reset_color(void)
{
  SetConsoleTextAttribute(stdout_handle, csbi.wAttributes);
}
