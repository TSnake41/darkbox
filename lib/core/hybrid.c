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

/* Use ansi backend or fallback to winapi otherwise.
   Safer than CORE_W10_ANSI but slower and more bloated (contains both implementation).
*/
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

#include <windows.h>

static bool use_ansi = false;

/* Include ANSI code */
#define CORE_W10_ANSI

#define core_init core_init_ansi
#define core_gotoxy core_gotoxy_ansi
#define core_gotoxy_relative core_gotoxy_relative_ansi
#define core_change_cursor_state core_change_cursor_state_ansi
#define core_clear_console core_clear_console_ansi
#define core_change_color core_change_color_ansi
#define core_reset_color core_reset_color_ansi
#include <core/ansi.c>
#undef core_init
#undef core_gotoxy
#undef core_gotoxy_relative
#undef core_change_cursor_state
#undef core_clear_console
#undef core_change_color
#undef core_reset_color

/* Include winapi code */
#define core_init core_init_winapi
#define core_gotoxy core_gotoxy_winapi
#define core_gotoxy_relative core_gotoxy_relative_winapi
#define core_change_cursor_state core_change_cursor_state_winapi
#define core_clear_console core_clear_console_winapi
#define core_change_color core_change_color_winapi
#define core_reset_color core_reset_color_winapi
#include <core/winapi.c>
#undef core_init
#undef core_gotoxy
#undef core_gotoxy_relative
#undef core_change_cursor_state
#undef core_clear_console
#undef core_change_color
#undef core_reset_color

void core_init(void)
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode;
  
  if (!GetConsoleMode(hOut, &mode)) {
    /* We are not in a console, possibly a pipe, use ansi. */
    use_ansi = true;
    return;
  }
  
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  use_ansi = SetConsoleMode(hOut, mode); /* use ansi only if mode is properly set .*/
  
  if (!use_ansi)
    core_init_winapi(); /* Initialize winapi backend. */
}

#define wrap_func(func) (use_ansi ? func##_ansi : func##_winapi)

void core_gotoxy(int x, int y)
{
  wrap_func(core_gotoxy)(x, y);
}

void core_gotoxy_relative(int x, int y)
{
  wrap_func(core_gotoxy_relative)(x, y);
}

void core_change_cursor_state(bool state)
{
  wrap_func(core_change_cursor_state)(state);
}

void core_clear_console(void)
{
  wrap_func(core_clear_console)();
}

void core_change_color(uint8_t color)
{
  wrap_func(core_change_color)(color);
}

void core_reset_color(void)
{
  wrap_func(core_reset_color)();
}