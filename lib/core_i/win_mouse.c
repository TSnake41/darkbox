/*
    Darkbox - A Fast and Portable Console IO Server
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met :

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the name of Teddy Astie (TSnake41) nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY TEDDY ASTIE AND CONTRIBUTORS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL TEDDY ASTIE AND CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
    LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <windows.h>

#include <core_i.h>

static char tomouse_b(DWORD, DWORD);

static HANDLE hin;
static int on_move;

void core_input_initialize(unsigned int mode)
{
    on_move = mode;
    hin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
}

void core_input_terminate(unsigned int mode)
{

}

void core_get_mouse(char on_move, int *x, int *y, int *b)
{
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

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
}

static char tomouse_b(DWORD m_bs, DWORD m_ef)
{
	if (m_bs & FROM_LEFT_1ST_BUTTON_PRESSED) /* left clic */
		return (m_ef & DOUBLE_CLICK) ? D_LEFT_BUTTON : LEFT_BUTTON;

	else if (m_bs & RIGHTMOST_BUTTON_PRESSED) /* right clic */
		return (m_ef & DOUBLE_CLICK) ? D_RIGHT_BUTTON : RIGHT_BUTTON;

	else if (m_bs & FROM_LEFT_2ND_BUTTON_PRESSED) /* middle clic */
		return MIDDLE_BUTTON;

	else if (m_ef & MOUSE_WHEELED) /* mouse scrolling */
		return HIWORD(m_bs) > 0 ? SCROLL_UP : SCROLL_DOWN;

	else /* mouse moved */
		return NOTHING;
}

void core_input_get_event(input_event_t *ie)
{
    DWORD e;
    INPUT_RECORD ir;

    retry:
    do
        ReadConsoleInput(hin, &ir, 1, &e);
    while(ir.EventType != MOUSE_EVENT && ir.EventType != KEY_EVENT);

    switch (ir.EventType) {
        case MOUSE_EVENT: ;
            COORD mouse_pos = ir.Event.MouseEvent.dwMousePosition;
            ie->type = MOUSE;
            ie->event.mouse.x = mouse_pos.X;
            ie->event.mouse.y = mouse_pos.Y;

			ie->event.mouse.b = tomouse_b(ir.Event.MouseEvent.dwButtonState, ir.Event.MouseEvent.dwEventFlags);
            break;

        case KEY_EVENT:
			if (!ir.Event.KeyEvent.bKeyDown)
				goto retry;

            ie->type = KEY_PRESS;
			/* Of course, this is an hack */
			ie->event.key_press = ir.Event.KeyEvent.uChar.AsciiChar
				? ir.Event.KeyEvent.uChar.AsciiChar
				: ir.Event.KeyEvent.wVirtualScanCode;
            break;
    }
}
