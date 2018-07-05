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
