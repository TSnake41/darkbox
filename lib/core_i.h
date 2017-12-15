/*
    Copyright (c) 2016-2017 Teddy ASTIE (TSnake41)

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

#ifndef H_CORE_I
#define H_CORE_I

#include <stdint.h>
#include <stdbool.h>

#if !defined __DJGPP__ && !defined WIN32
/* Declare getch() and kbhit()
   See core_i/unix_conio.c.
*/
int getch();
int kbhit();
#else
/* Use DOS-Like headers. */
#include <conio.h>
#endif

/* Event type */
enum core_event_type {
    MOUSE = 1,
    KEY_PRESS = 2
};

typedef struct core_mouse_event {
    unsigned int x, y, b;
} core_mouse_event;

typedef struct core_input_event {
    int type;
    union {
        core_mouse_event mouse;
        unsigned char key_press;
    } event;
} core_input_event;

/* Mouse input enumeration */
enum {
	NOTHING = 0,
    LEFT_BUTTON,
    RIGHT_BUTTON,
    D_LEFT_BUTTON,
    D_RIGHT_BUTTON,
    MIDDLE_BUTTON,
    SCROLL_UP,
    SCROLL_DOWN,
    RELEASE
};

#define core_getkey getch
#define core_kbhit kbhit

void core_get_mouse(bool, unsigned int *, unsigned int *, unsigned int *);
void core_input_get_event(core_input_event *e);

void core_mouse_initialize(bool on_move);
void core_mouse_terminate(bool on_move);

bool core_is_stdin_console(void);

#ifndef WIN32
/* *NIX kbhit works in this case. */
#define core_peek_stdin core_kbhit
#else
int core_peek_stdin(void);
#endif

#endif /* H_CORE_I */
