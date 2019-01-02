/*
  Copyright (C) 2016-2019 Teddy ASTIE

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

/* Darkbox console output core library */

#if !defined(_XOPEN_SOURCE) && !defined(WIN32)
#define _XOPEN_SOURCE 700
#endif

#include <stdint.h>
#include <stdio.h>

#ifndef WIN32
#include <unistd.h>
#include <time.h>
#else
#include <windows.h>
#endif

#ifdef CORE_W10_ANSI
#define CORE_USE_ANSI
#endif

#if !defined(CORE_USE_ANSI) && !defined(CORE_USE_WINAPI) && !defined(CORE_NO_CONSOLE)
  #ifndef WIN32
  #define CORE_USE_ANSI
  #else
  #define CORE_USE_WINAPI
  #endif
#endif

#if defined(CORE_USE_WINAPI)
#include <core/winapi.c>
#elif defined(CORE_USE_ANSI)
#include <core/ansi.c>
#else
#include <core/dummy.c>
#endif

void core_swritecolor(uint8_t color, const char *s)
{
  if (!s)
    return;

  core_change_color(color);
  fputs(s, stdout);

  core_reset_color();
}

void core_cwritecolor(uint8_t color, int c)
{
  core_change_color(color);
  putchar(c);

  core_reset_color();
}

void core_sleep(unsigned long ms)
{
  #if defined(__DJGPP__)
  delay(ms);
  #elif defined(WIN32)
  Sleep(ms);
  #else
  struct timespec req;
  time_t sec = (int)(ms / 1000);
  ms -= sec * 1000;
  req.tv_sec = sec;
  req.tv_nsec = ms * 1e+6L;
  while (nanosleep(&req, &req) == -1)
    continue;
  #endif
}