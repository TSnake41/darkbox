/*
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

#ifndef H_CORE
#define H_CORE

#include <stdint.h>
#include <stdbool.h>

void core_init(void);

void core_swritecolor(uint8_t color, const char *s);
void core_cwritecolor(uint8_t color, int c);
void core_gotoxy_relative(int x_offset, int y_offset);

void core_gotoxy(int x, int y);
void core_change_color(uint8_t color);
void core_change_cursor_state(bool state);
void core_clear_console(void);
void core_reset_color(void);

void core_sleep(unsigned long ms);

#endif /* H_CORE */
