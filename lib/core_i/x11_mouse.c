/*
  Copyright (C) 2017-2018 Teddy ASTIE

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

#include <core_i.h>

static char tomouse_b(int mouse_char);
static char latest;

void core_mouse_initialize(bool on_move)
{
  fprintf(stderr, "\033[?%dh", on_move ? 1003 : 1000);
}

void core_mouse_terminate(bool on_move)
{
  fprintf(stderr, "\033[?%dl", on_move ? 1003 : 1000);
}

static char tomouse_b(int mouse_char)
{
  /* Update *latest and return button. */
  #define return_button(button) return (latest = (CORE_##button))
  /* The same with D_ (double click) support. */
  #define return_button_dc(button) \
    return (latest = (latest == CORE_##button ? CORE_D_##button : CORE_##button))

	switch (mouse_char) {
      case ' ': /* Left button */
      case '@':
        return_button_dc(LEFT_BUTTON);

      case '"': /* Right button */
      case 'B':
        return_button_dc(RIGHT_BUTTON);

      case '!': /* Middle button */
      case 'A':
        return_button(MIDDLE_BUTTON);

      case '`': /* Scroll up */
        return_button(SCROLL_UP);

      case 'a': /* Scroll down */
        return_button(SCROLL_DOWN);

      case '#': /* Mouse release */
        /* Do not redefine *latest */
        return CORE_RELEASE;

      case 'C': /* Nothing (1003 mode only) */
        return_button(NOTHING);

      default:
        return -1;
	}
}

void core_input_get_event(core_input_event *e)
{
  int c = core_getkey();

  if (c == CORE_KEY_MOUSE) {
  	/* Mouse event */
    e->type = CORE_EVENT_MOUSE;
    e->event.mouse.b = tomouse_b(core_getkey());
    e->event.mouse.x = core_getkey() - 33;
    e->event.mouse.y = core_getkey() - 33;
  } else {
    /* Key press */
    e->type = CORE_EVENT_KEYBOARD;
    e->event.key_press = c;
  }
}

void core_get_mouse(bool on_move, unsigned int *x, unsigned int *y, unsigned int *b)
{
  core_mouse_initialize(on_move);

  int c;
  do /* Wait CSI mouse start (-2, see posix_conio.c) */
    c = core_getkey();
  while(c != CORE_KEY_MOUSE);

  *b = tomouse_b(core_getkey());

  *x = core_getkey() - 33;
  *y = core_getkey() - 33;

  core_mouse_terminate(on_move);
}
