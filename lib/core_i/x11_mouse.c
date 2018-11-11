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

static int tomouse_b(int mouse_char);
static int latest;

void core_mouse_initialize(bool on_move)
{
  fprintf(stderr, "\033[?%dh", on_move ? 1003 : 1000);
  latest = CORE_RELEASE;
}

void core_mouse_terminate(bool on_move)
{
  fprintf(stderr, "\033[?%dl", on_move ? 1003 : 1000);
}

static int tomouse_b(int b)
{
  /* Update latest if needed and return button. */
  #define return_button(button, redefine_latest) do { \
    if (redefine_latest) latest = CORE_##button; \
    return CORE_##button; \
  } while (0)

  /* The same with D_ (double click) support. */
  #define return_button_dc(button, redefine_latest) do { \
    enum core_mouse_button temp_btn = (latest == CORE_##button) ? CORE_D_##button : CORE_##button; \
    if (redefine_latest) latest = temp_btn; \
    return temp_btn; \
  } while (0)

  /* See extras/doc/xterm-mouse-tracking-analysis.ods for more informations. */
  b -= 32;

  int btn = b & 0x3;

  /* Check scrolling flag */
  if (b & (1 << 6)) {
    /* Reset latest with a neutral/non-significant value. */
    latest = CORE_NOTHING;
    /* Currently scrolling, but up or down ? */
    /* NOTE: No idea if there is another value for the btn flag. */
    return (btn == 1) ? CORE_SCROLL_DOWN : CORE_SCROLL_UP;
  }

  /* Check movement flag */
  bool moving = b & (1 << 5);

  if (moving)
    /* Reset latest */
    latest = CORE_RELEASE;

  switch (btn) {
    case 0:
      return_button_dc(LEFT_BUTTON, !moving);
      break;

    case 1:
      return_button(MIDDLE_BUTTON, !moving);
      break;

    case 2:
      return_button_dc(RIGHT_BUTTON, !moving);
      break;

    case 3:
      if (moving)
        return_button(NOTHING, false);
      else
        return_button(RELEASE, false);
      break;
  }

  /* Should not be achieved. */
  return -1;
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

void core_get_mouse(bool on_move, int *x, int *y, int *b)
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
