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

#include <stdbool.h>

#include <windows.h>
#include <io.h>

int core_getkey(void)
{
  int key = getch();
  if (key == 224 || key == 0)
    return 255 + getch();
}

int core_peek_stdin(void)
{
  /* Threat stdin as a pipe, curiously, this works,
    however this do not follow Windows's philosophy.
  */
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD available = 0;
  PeekNamedPipe(hin, NULL, 0, NULL, &available, NULL);
  return available;
}

bool core_is_stdin_console(void)
{
  HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
  DWORD mode;

  return GetConsoleMode(hin, &mode) != 0;
}
