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
/* Darkbox Console Output Core */

#include <stdio.h>
#include <stdlib.h>
/* #include <assert.h> */

#include "core.h"

#ifdef WIN32
#include <windows.h>

static HANDLE stdout_handle;
static CONSOLE_SCREEN_BUFFER_INFO csbi;
#else

const char Ansi_Table[8] = {
    0, 4, 2, 6,
    1, 5, 3, 7,
};

#endif

#if defined(WIN32) || defined(__DJGPP__)
#include <conio.h>
#endif

void core_init(void)
{
    #ifdef WIN32
    stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(stdout_handle, &csbi);
    #endif
}

void core_gotoxy(int x, int y)
{
    /* assert(x > -1 || y > -1); */
    if (x < 0 || y < 0)
        return;

    #ifndef WIN32
    printf("\033[%d;%df", y + 1, x + 1);
    #else
    COORD c = { x, y };
    SetConsoleCursorPosition(stdout_handle, c);
    #endif
}

void core_gotoxy_relative(int x, int y)
{
    #ifndef WIN32
    if (x != 0)
        printf("\033[%d%c",
            abs(x),
            x > 0 ? 'C' : 'D'
        );


    if (y != 0)
        printf("\033[%d%c",
            abs(y),
            y > 0 ? 'B' : 'A'
        );

    #else
    GetConsoleScreenBufferInfo(stdout_handle, &csbi);

    core_gotoxy(
        max(0, csbi.dwCursorPosition.X + x),
        max(0, csbi.dwCursorPosition.Y + y)
    );
    #endif
}

void core_change_cursor_state(char state)
{
    #ifndef WIN32
    fputs (
        state ?
        "\033[?25h" : /* hide */
        "\033[?25l",  /* show */
        stdout
    );
    #else
    CONSOLE_CURSOR_INFO cursor;
    /* assert(GetConsoleCursorInfo(stdout_handle, &cursor)); */
    if (!GetConsoleCursorInfo(stdout_handle, &cursor))
        return;

    cursor.bVisible = (state > 0);
    SetConsoleCursorInfo(stdout_handle, &cursor);
    #endif
}


void core_clear_console(void)
{
    #ifndef WIN32
    fputs("\033[2J", stdout);
    #else
    /* Based on libDos9's, Dos9_ClearConsoleScreen function
       Copyright (C) 2010-2016 Romain Garbi
    */

    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten, dwConSize;

    GetConsoleScreenBufferInfo( stdout_handle, &csbi );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    FillConsoleOutputCharacter( stdout_handle, (TCHAR) ' ',
       dwConSize, coordScreen, &cCharsWritten );

    GetConsoleScreenBufferInfo( stdout_handle, &csbi );
    FillConsoleOutputAttribute( stdout_handle, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
    #endif

    core_gotoxy(0, 0);
}

void core_change_color(unsigned char color)
{
    #ifndef WIN32
    char bc = color & 0xF;
    char fc = color >> 4;

    printf("\033[0m\033[%s3%dm\033[%s4%dm",
        bc > 7 ? "1;" : "", Ansi_Table[bc % 8],
        fc > 7 ? "1;" : "", Ansi_Table[fc % 8]
    );
    #else
    SetConsoleTextAttribute(stdout_handle, color);
    #endif
}

void core_reset_color(void)
{
    #ifndef WIN32
    fputs("\033[39m\033[49m", stdout);
    #else
    SetConsoleTextAttribute(stdout_handle, csbi.wAttributes);
    #endif
}

void core_swritecolor(unsigned char color, const char *s)
{
    /* assert(!s); */
    if (!s)
        return;

    core_change_color(color);
    fputs(s, stdout);

    core_reset_color();
}

void core_cwritecolor(unsigned char color, int c)
{
    core_change_color(color);
    putchar(c);

    core_reset_color();
}
