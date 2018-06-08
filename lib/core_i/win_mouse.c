/*
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <stdbool.h>
#include <windows.h>

#include <core_i.h>

static char tomouse_b(DWORD, DWORD);

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

void core_get_mouse(bool on_move, unsigned int *x, unsigned int *y, unsigned int *b)
{
  core_mouse_initialize(on_move);
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);

	DWORD e;
	INPUT_RECORD ir;

	*b = NOTHING;

	do {
		do
			ReadConsoleInput(hin, &ir, 1, &e);
		while (ir.EventType != MOUSE_EVENT);

		COORD mouse_pos = ir.Event.MouseEvent.dwMousePosition;
		*x = mouse_pos.X;
		*y = mouse_pos.Y;

		*b = tomouse_b(ir.Event.MouseEvent.dwButtonState, ir.Event.MouseEvent.dwEventFlags);
	} while (!on_move && *b == NOTHING);

    core_mouse_terminate(on_move);
}

static char tomouse_b(DWORD m_bs, DWORD m_ef)
{
  static char latest = NOTHING;

  /* Redefine latest and return button. */
  #define return_button(button) return (latest = (button))

	if (m_bs & FROM_LEFT_1ST_BUTTON_PRESSED) /* left clic */
		return_button((m_ef & DOUBLE_CLICK) ? D_LEFT_BUTTON : LEFT_BUTTON);

	else if (m_bs & RIGHTMOST_BUTTON_PRESSED) /* right clic */
		return_button((m_ef & DOUBLE_CLICK) ? D_RIGHT_BUTTON : RIGHT_BUTTON);

	else if (m_bs & FROM_LEFT_2ND_BUTTON_PRESSED) /* middle clic */
		return_button(MIDDLE_BUTTON);

	else if (m_ef & MOUSE_WHEELED) /* mouse scrolling */
		return (int)m_bs < 0 ? SCROLL_UP : SCROLL_DOWN;

	else {
    /* mouse moved */
    if (latest != NOTHING) {
      /* Button release */
      latest = NOTHING;
      return RELEASE;
    }

  	return NOTHING;
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
    		ie->type = MOUSE;
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

    		ie->type = KEY_PRESS;
        int input = getch();
        if (input == 224)
          input = getch();
    		ie->event.key_press = input;
        event_pulled = true;
    		break;
    }
	} while (!event_pulled);
}
