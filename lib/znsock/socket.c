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

#include <stddef.h>
#include <stdint.h>
#include <znsock.h>

#ifndef ZNSOCK_CLOSE_TIMEOUT
#define ZNSOCK_CLOSE_TIMEOUT 20000
#endif

#if (!defined(WIN32) || defined(FORCE_POLL)) && !defined(FORCE_SELECT)
#define ZNSOCK_USE_POLL
#else
#define ZNSOCK_USE_SELECT
#endif

int znsock_recv_block(znsock s, void *buffer, size_t size, bool blocking)
{
  #ifndef WIN32
  /* Use MSG_WAITALL */
  return recv(s, buffer, size, blocking ? MSG_WAITALL : 0);
  #else
  if (blocking) {
    size_t readed = 0;
    do {
      int ret = recv(s, buffer + readed, size - readed, 0);
      if (ret <= 0)
        /* Something went wrong... */
        return ret;

      readed += ret;
    } while(size > readed);

    return readed;
  } else
    return recv(s, buffer, size, 0);
  #endif
}

void znsock_close(znsock s, bool graceful)
{
  if (graceful && shutdown(s, SHUT_WR) == 0) {
    /* Okay, we shutdowned socket, now, we have to wait until all on-fly
       data are received by pair, however, we must make sure that this check
       does not idle eternally using a timeout (with polling ;)).
    */
    #ifdef ZNSOCK_USE_POLL
    struct pollfd fd = {
      .fd = s,
      .events = POLLIN
    };
    #else /* ZNSOCK_USE_SELECT */
    fd_set set;
    FD_ZERO(&set);

    FD_SET(s, &set);

    struct timeval tv;
    tv.tv_sec = ZNSOCK_CLOSE_TIMEOUT / 1000;
    tv.tv_usec = (ZNSOCK_CLOSE_TIMEOUT % 1000) * 1000;
    #endif

    #ifdef ZNSOCK_USE_POLL
    if (poll(&fd, 1, ZNSOCK_CLOSE_TIMEOUT) == 1) {
    #else /* ZNSOCK_USE_SELECT */
    if (select(1, &set, NULL, NULL, &tv) == 1) {
    #endif
      /* Make sure peer received everything. */
      uint8_t buf;
      znsock_recv(s, &buf, 1);
    }
  }
  /* Close socket. */
  close(s);
}
