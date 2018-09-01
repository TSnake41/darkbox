/*
  Copyright (C) 2018 Teddy ASTIE

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

#include <signal.h>
#include <stdbool.h>
#include <znsock.h>

bool znsock_init(void)
{
  #ifdef WIN32
  WSADATA wsa;
  return WSAStartup(MAKEWORD(2, 2), &wsa) != 0;
  #else
  /* Ignore broken pipe signals. */
  signal(SIGPIPE, SIG_IGN);
  return false;
  #endif
}

void znsock_cleanup(void)
{
  #ifdef WIN32
  WSACleanup();
  #endif
}
