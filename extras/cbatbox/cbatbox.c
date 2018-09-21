/*
   Copyright (C) 2017 ASTIE Teddy (TSnake41)
 
   Permission to use, copy, modify, and/or distribute this software for any
   purpose with or without fee is hereby granted, provided that the above
   copyright notice and this permission notice appear in all copies.
 
   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
   WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
   MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
   ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
   OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
 
//#define _WIN32_WINNT 0x0500
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
 
#ifdef USE_BB_FILE
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif
 
int __getmainargs(int *, char ***, char ***, int, void *);
 
static void Key_(void);
static void Goto_(void);
static void Char_(void);
static void Color_(void);
static void Disp_(void);
static void Mouse_(void);
static void Offset_(void);
static void Wait_(void);
static void Hide_(void);
static void HideConsole_(void);
static void Position_(void);
static void NewLine_(void);
 
#ifdef USE_X_OPTION
static void Sprite_(void);
#endif
 
void (*jumpTable[])(void) = {
    Key_, Goto_, Char_,
    Color_, Disp_, Mouse_,
    Offset_, Wait_, Hide_,
    HideConsole_,Position_,NewLine_,
    #ifdef USE_X_OPTION
    Sprite_
    #endif
};
 
#ifdef USE_X_OPTION
const char *jumpChar = "kgacdmowhpynx";
#else
const char *jumpChar = "kgacdmowhpyn";
#endif
 
static void argsLoop_(void);
static void LabelLoop_(char);
static void EndCommand_(void);
#define EndCommand_quick_() exit(status)
static INPUT_RECORD GetMouseInput_(void);
static void PrintMouseCoord_(INPUT_RECORD);
static void GetNextArg_(void);
static int GetNextNumber_(void);
 
unsigned long bin;
int argc, status = 0;
char **argv;
HANDLE hIn, hOut;
 
#ifdef USE_BB_FILE
int fd;
#endif
 
struct {
    int dwXOffset;
    int dwYOffset;
    int dwXLast;
    int dwYLast;
    char pBuf[5];
} bb_info = { 0 };
 
void _start(void)
{
    __getmainargs(&argc, &argv, (void *)&bin, 0, (void *)&bin);
 
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hIn = GetStdHandle(STD_INPUT_HANDLE);
 
    #ifdef USE_BB_FILE
    fd = _open(".bb", O_RDWR | O_CREAT | O_BINARY, S_IWRITE | S_IREAD);
 
    if (fd == -1)
        EndCommand_quick_();
 
    _read(fd, &bb_info, 16);
    #endif
 
    argsLoop_();
}
 
static void argsLoop_(void)
{
    while (true) {
        GetNextArg_();
        char c = (*argv)[1] | 0x20;
 
        LabelLoop_(c);
    }
}
 
static void LabelLoop_(char c)
{
    /* Includes ChooseJump. */
 
    /* NOTE: Lookup jumpChar as NUL-terminated. */
    int i = 0;
    while (jumpChar[i]) {
        if (c == jumpChar[i]) {
            /* ChooseJump_ */
            jumpTable[i]();
            break;
        }
        i++;
    }
}
 
static void EndCommand_(void)
{
    #ifdef USE_BB_FILE
    _chsize(fd, 0);
    _write(fd, &bb_info, 16);
    _close(fd);
    #endif
 
    EndCommand_quick_();
}
 
static INPUT_RECORD GetMouseInput_(void)
{
    INPUT_RECORD input;
    MOUSE_EVENT_RECORD *m = &input.Event.MouseEvent;
 
    do {
        SetConsoleMode(hIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
        ReadConsoleInput(hIn, &input, 1, &bin);
    } while (bin == 0 || input.EventType != MOUSE_EVENT);
 
    /* Small hack(s) (I think). */
    if (m->dwEventFlags & MOUSE_WHEELED)
        m->dwButtonState = 5;
 
    if (m->dwEventFlags & MOUSE_MOVED && m->dwButtonState == 0)
        m->dwButtonState = 2;
 
    return input;
}
 
static void PrintMouseCoord_(INPUT_RECORD input)
{
    /* Original code is more complex since it
       uses some tricks to get X and Y.
    */
    MOUSE_EVENT_RECORD m = input.Event.MouseEvent;
 
    printf("%d:%d:%d",
        m.dwMousePosition.X,
        m.dwMousePosition.Y,
        (int)m.dwButtonState
    );
}
 
static void GetNextArg_(void)
{
    argv++;
 
    if (*argv == NULL)
        EndCommand_();
}
 
static int GetNextNumber_(void)
{
    GetNextArg_();
    return strtol(*argv, NULL, 0);
}
 
static void Key_(void)
{
    int key;
 
    if ((*argv)[2] == '_' && !kbhit())
        return;
 
    /* KeyNext_ */
    if ((key = getch()) == 224)
        /* Extended key */
        key = getch() + 255;
 
    status = key;
    EndCommand_();
}
 
static void Goto_(void)
{
    COORD p = {
        .X = GetNextNumber_() + bb_info.dwXOffset,
        .Y = GetNextNumber_() + bb_info.dwYOffset
    };
 
    bb_info.dwXLast = p.X;
    bb_info.dwYLast = p.Y;
 
    /* Goto_simple_ for Goto_ */
    SetConsoleCursorPosition(hOut, p);
}
 
static void Char_(void)
{
    /* A bit complex... */
    *((int *)bb_info.pBuf) = GetNextNumber_();
    printf("%s", bb_info.pBuf);
}
 
static void Color_(void)
{
    SetConsoleTextAttribute(hOut, GetNextNumber_());
}
 
static void Disp_(void)
{
    GetNextArg_();
    printf("%s", *argv);
}
 
static void Mouse_(void)
{
    INPUT_RECORD input;
    MOUSE_EVENT_RECORD m;
 
    do {
        input = GetMouseInput_();
        m = input.Event.MouseEvent;
    } while (m.dwButtonState == 0 || m.dwButtonState == 5);
 
    PrintMouseCoord_(input);
}
 
static void Offset_(void)
{
    bb_info.dwXOffset = GetNextNumber_();
    bb_info.dwYOffset = GetNextNumber_();
}
 
static void Wait_(void)
{
    Sleep(GetNextNumber_());
}
 
static void Hide_(void)
{
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = GetNextNumber_();
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
 
static void HideConsole_(void)
{
    ShowWindow(GetConsoleWindow(), GetNextNumber_());
}
static void Position_(void)
{
    PrintMouseCoord_(GetMouseInput_());
}
 
static void NewLine_(void)
{
    SetConsoleCursorPosition(hOut, (COORD){ bb_info.dwXLast, ++bb_info.dwYLast });
}
 
#ifdef USE_X_OPTION
static void Sprite_(void)
{
    GetNextArg_();
    int fd2 = _open(*argv, O_RDONLY | O_BINARY);
    if (fd2 == -1)
        return;
 
    COORD dim, p = {
        .X = GetNextNumber_(),
        .Y = GetNextNumber_()
    };
 
    _read(fd2, &dim, 4);
 
    unsigned int px_count = dim.X * dim.Y;
    if (px_count > 0x2fffffff)
        return;
 
    px_count <<= 2;
 
    char *chr_buffer = malloc(px_count);
    if (chr_buffer == NULL)
        goto Sprite_Close_;
 
    if (_read(fd2, chr_buffer, px_count) != px_count)
        goto Sprite_Skip_Render_;
 
    SMALL_RECT rect = {
        .Left = p.X,
        .Top = p.Y,
        .Right = p.X + dim.X,
        .Bottom = p.Y + dim.Y
    };
 
    WriteConsoleOutput(hOut, chr_buffer, dim, p, &rect);
 
    Sprite_Skip_Render_:
        free(spr_block);
 
    Sprite_Close_:
        _close(fd2);
}
#endif
 