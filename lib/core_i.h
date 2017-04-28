/*

 Darkbox - A Fast and Portable Console IO Server
 Copyright (c) 2016 Teddy ASTIE (TSnake41)

 All rights reserved.
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

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

#ifndef H_CORE_I
#define H_CORE_I

#if !defined __DJGPP__ && !defined WIN32
/* Declare getch() and kbhit()
   See core_i/unix_conio.c
*/
int getch();
int kbhit();
#else
/* Use DOS-Like headers. */
#include <conio.h>
#endif

/* Event type */
enum event_type {
    MOUSE = 1,
    KEY_PRESS = 2
};

typedef struct mouse_event {
    int x, y, b;
} mouse_event_t;



typedef struct input_event {
    int type;
    union {
        mouse_event_t mouse;
        unsigned char key_press;
    } event;
} input_event_t;

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

	/* Planned :

		Grabbing :
		  - G_LEFT_BUTTON
		  - G_RIGHT_BUTTON
		  - G_MIDDLE_BUTTON

	 */
};

#define core_getkey() getch()
#define core_kbhit() kbhit()

void core_get_mouse(char, int*, int*, int*);
void core_input_get_event(input_event_t *e);

void core_input_initialize(unsigned int mode);
void core_input_terminate(unsigned int mode);

#endif /* H_CORE_I */
