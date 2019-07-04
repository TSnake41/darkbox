/*
  Copyright (C) 2017-2019 Teddy ASTIE

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
#include <unistd.h>

#include <ctype.h>
#include <core_i.h>

/*
   Note 2019 :
    That's a pretty big deal to rewrite it, because special keys sequences
    are hard to parse and they varies between terminals.

    That way, the support is focused on mostly-used terminals, such as
    gnome-terminal, xfce4-terminal, lxterminal, and standard console buffer.

    Some other terminals might not work properly in some cases like aterm or rxvt.
*/

struct chartable_entry {
  short c;
  short keycode;
};

static const struct chartable_entry entries[] = {
  { 'M', -2 }, /* Mouse input beginning sequence. */
  { 'A', 327 }, /* up arrow */
  { 'B', 335 }, /* down arrow */
  { 'C', 332 }, /* right arrow */
  { 'D', 330 }, /* left arrow */
  { 'F', 334 }, /* begin */
  { 'H', 326 }, /* end */
  { 'P', 314 }, /* F1 */
  { 'Q', 315 }, /* F2 */
  { 'R', 316 }, /* F3 */
  { 'S', 317 }, /* F4 */
};
static const size_t entry_count = sizeof(entries) / sizeof(*entries);

static const struct chartable_entry spec_entries[] = {
  { 2, 326 }, /* insert */
  { 3, 338 }, /* del */
  { 5, 328 }, /* pg up */
  { 6, 336 }, /* pg down */
  { 7, 334 }, /* begin (aterm) */
  { 8, 326 }, /* end (aterm) */
  { 15, 318 }, /* F5 */
  { 17, 319 }, /* F6 */
  { 18, 320 }, /* F7 */
  { 19, 321 }, /* F8 */
  { 20, 322 }, /* F9 */
  { 21, 323 }, /* F10 */
  { 23, 388 }, /* F11 */
  { 24, 389 }, /* F12 */
};
static const size_t spec_entry_count = sizeof(spec_entries) / sizeof(*spec_entries);

static bool isspec(int c)
{
  return c == '~' || c == '$' || c == '^';
}

int core_getkey(void)
{
  int ch = getch();

  if (ch == 0x1B) {
    /* Escape sequence encountered, handle special characters */
    int next = getch();

    /* Alt+key behavior for aterm and alike. */
    if (next == 0x1B)
      next = getch();

    /* Most keys outputs [ after. Some terminals may output 'O' instead. */
    if (next == '[' || next == 'O')
      next = getch();

    /* The "parameter" is the first number of the sequence.
       It is mostly used for some keys like Insert, Page up/down, ... */
    int param = 0;
    bool param_readed = false;

    while (!isalpha(next) && !isspec(next)) {
      param_readed = param_readed || next == ';';

      if (!param_readed) {
        /* Read param number. */
        int i = next - '0';
        param = param == 0 ? i : 10 * param + i;
      }

      next = getch();
    }

    if (isspec(next)) {
      for (size_t i = 0; i < spec_entry_count; i++)
        if (spec_entries[i].c == param)
          return spec_entries[i].keycode;
    } else
      for (size_t i = 0; i < entry_count; i++)
        if (entries[i].c == next)
          return entries[i].keycode;
  } else
    return ch;

  return -1;
}

bool core_is_stdin_console(void)
{
  return isatty(fileno(stdin));
}
