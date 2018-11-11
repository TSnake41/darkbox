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
enum {
  CORE_EVENT_MOUSE = 1,
  CORE_EVENT_KEYBOARD = 2
};

typedef struct core_mouse_event {
    int x, y, b;
} core_mouse_event;

typedef struct core_input_event {
    int type;
    union {
        core_mouse_event mouse;
        int key_press;
    } event;
} core_input_event;

/* Mouse input enumeration */
enum core_mouse_button {
	CORE_NOTHING = 0,
    CORE_LEFT_BUTTON,
    CORE_RIGHT_BUTTON,
    CORE_D_LEFT_BUTTON,
    CORE_D_RIGHT_BUTTON,
    CORE_MIDDLE_BUTTON,
    CORE_SCROLL_UP,
    CORE_SCROLL_DOWN,
    CORE_RELEASE
};

/* core_i special keys */
enum {
  CORE_KEY_ERROR = -1,
  CORE_KEY_UNKNOWN = -1, /* same as CORE_KEY_ERROR */
  CORE_KEY_MOUSE = -2, /* *NIX only */
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
