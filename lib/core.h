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

#ifndef H_CORE
#define H_CORE

#include <stdbool.h>

void core_init(void);

void core_swritecolor(unsigned char color, const char *s);
void core_cwritecolor(unsigned char color, int c);
void core_gotoxy_relative(int x_offset, int y_offset);

void core_gotoxy(int x, int y);
void core_change_color(unsigned char color);
void core_change_cursor_state(bool state);
void core_clear_console(void);
void core_reset_color(void);

#ifndef WIN32
void core_sleep(unsigned long ms);
#else
#include <windows.h>

#define core_sleep Sleep
#endif

#endif /* H_CORE */
