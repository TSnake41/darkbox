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
	
    if (c == -2) {
		/* Mouse event */
        e->type = MOUSE;
        e->event.mouse.b = tomouse_b(core_getkey());
        e->event.mouse.x = core_getkey() - 33;
        e->event.mouse.y = core_getkey() - 33;
	} else {
		/* Key press */
		e->type = KEY_PRESS;
		e->event.key_press = c;
	}
}

void core_get_mouse(bool on_move, unsigned int *x, unsigned int *y, unsigned int *b)
{
    core_mouse_initialize(on_move);

    int c;
    do /* Wait CSI mouse start (-2, see posix_conio.c) */
        c = core_getkey();
    while(c != -2);

    *b = tomouse_b(core_getkey());

    *x = core_getkey() - 33;
    *y = core_getkey() - 33;

    core_mouse_terminate(on_move);
}
