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

#include <stdbool.h>
#include <windows.h>

#include <core_i.h>

static int tomouse_b(DWORD, DWORD);

static DWORD flags;
static bool on_move, mouse_enabled = false;

void core_mouse_initialize(bool on_move_arg)
{
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  mouse_enabled = true;
  on_move = on_move_arg;

  /* Save old flags */
  GetConsoleMode(hin, &flags);

  /* Set new flags */
  const DWORD new_flags =
    ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT;

  SetConsoleMode(hin, new_flags);
}

void core_mouse_terminate(bool on_move)
{
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  mouse_enabled = false;
  SetConsoleMode(hin, flags);
}

void core_get_mouse(bool on_move, int *x, int *y, int *b)
{
  core_mouse_initialize(on_move);
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

	DWORD e;
	INPUT_RECORD ir;

	*b = CORE_NOTHING;

	do {
		do
			ReadConsoleInput(hin, &ir, 1, &e);
		while (ir.EventType != MOUSE_EVENT);

		COORD mouse_pos = ir.Event.MouseEvent.dwMousePosition;
		*x = mouse_pos.X;
		*y = mouse_pos.Y;

		*b = tomouse_b(ir.Event.MouseEvent.dwButtonState, ir.Event.MouseEvent.dwEventFlags);
	} while (!on_move && *b == CORE_NOTHING);

    core_mouse_terminate(on_move);
}

static int tomouse_b(DWORD m_bs, DWORD m_ef)
{
  static int latest = CORE_NOTHING;

  /* Redefine latest and return button. */
  #define return_button(button) return (latest = (button))

  if (m_bs & FROM_LEFT_1ST_BUTTON_PRESSED) /* left clic */
    return_button((m_ef & DOUBLE_CLICK)
      ? CORE_D_LEFT_BUTTON
      : CORE_LEFT_BUTTON);

  else if (m_bs & RIGHTMOST_BUTTON_PRESSED) /* right clic */
    return_button((m_ef & DOUBLE_CLICK)
      ? CORE_D_RIGHT_BUTTON
      : CORE_RIGHT_BUTTON);

  else if (m_bs & FROM_LEFT_2ND_BUTTON_PRESSED) /* middle clic */
      return_button(CORE_MIDDLE_BUTTON);

  else if (m_ef & MOUSE_WHEELED) /* mouse scrolling */
      return (int)m_bs < 0 ? CORE_SCROLL_UP : CORE_SCROLL_DOWN;

  else {
    /* mouse moved */
    if (latest != CORE_NOTHING) {
      /* Button release */
      latest = CORE_NOTHING;
      return CORE_RELEASE;
    }

    return CORE_NOTHING;
  }
}

void core_input_get_event(core_input_event *ie)
{
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD e;
  INPUT_RECORD ir;

  bool event_pulled = false;

  do {
    ReadConsoleInput(hin, &ir, 1, &e);

    switch (ir.EventType) {
      case MOUSE_EVENT:
        if (!mouse_enabled)
          /* Mouse mode disabled, skip event. */
          continue;

        COORD mouse_pos = ir.Event.MouseEvent.dwMousePosition;
        ie->type = CORE_EVENT_MOUSE;
        ie->event.mouse.x = mouse_pos.X;
        ie->event.mouse.y = mouse_pos.Y;

        DWORD m_bs = ir.Event.MouseEvent.dwButtonState,
              m_ef = ir.Event.MouseEvent.dwEventFlags;

        ie->event.mouse.b = tomouse_b(m_bs, m_ef);
        event_pulled = true;
        break;

      case KEY_EVENT:
        if (!ir.Event.KeyEvent.bKeyDown)
          continue;

        /* Push event to the stack (to get working
           getch()), however, it breaks input order.
        */
        WriteConsoleInputA(hin, &ir, 1, &e);

        ie->type = CORE_EVENT_KEYBOARD;
        ie->event.key_press = core_getkey();
        event_pulled = true;
        break;
      }
	} while (!event_pulled);
}
