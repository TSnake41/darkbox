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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <core_i.h>

static char tomouse_b(int mouse_char);
static char latest;

void core_input_initialize(bool on_move)
{
    fprintf(stderr, "\033[?%dh", on_move ? 1003 : 1000);
}

void core_input_terminate(bool on_move)
{
    fprintf(stderr, "\033[?%dl", on_move ? 1003 : 1000);
}

static char tomouse_b(int mouse_char)
{
    /* Update *latest and return button. */
    #define return_button(button) return (latest = (button))
    /* The same with D_ (double click) support. */
    #define return_button_dc(button) \
        return (latest = (latest == button ? D_##button : button))

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
            return RELEASE;

		case 'C': /* Nothing (1003 mode only) */
			return_button(NOTHING);

		default:
			return -1;
	}
}

void core_input_get_event(core_input_event *e)
{
    int c = core_getkey();
    if (c == -1)
        goto mouse;

    /* key_press */
    e->type = KEY_PRESS;
    e->event.key_press = c;
    return;

    mouse:
        e->type = MOUSE;
        e->event.mouse.b = tomouse_b(core_getkey());
        e->event.mouse.x = core_getkey() - 33;
        e->event.mouse.y = core_getkey() - 33;
        return;
}

void core_get_mouse(char on_move, int *mouse_x, int *mouse_y, int *mouse_b)
{
    core_input_initialize(on_move);

    int c;
    do /* Wait CSI mouse start (-1) */
        c = core_getkey();
    while(c != -1);

    *mouse_b = tomouse_b(core_getkey());

    *mouse_x = core_getkey() - 33;
    *mouse_y = core_getkey() - 33;

    core_input_terminate(on_move);
}
