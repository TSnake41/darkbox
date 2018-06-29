#include <znsock.h>

#ifndef ZNSOCK_CLOSE_TIMEOUT
#define ZNSOCK_CLOSE_TIMEOUT 20000
#endif

#if (!defined(WIN32) || defined(FORCE_POLL)) && !defined(FORCE_SELECT)
#define ZNSOCK_USE_POLL
#else
#define ZNSOCK_USE_SELECT
#endif

void znsock_close(znsock socket, bool graceful)
{
  if (graceful && shutdown(socket, SHUT_RDWR) == 0) {
    /* Okay, we shutdowned socket, now, we have to wait until all on-fly
       data are received by pair, however, we must make sure that this check
       does not idle eternally using a timeout (with polling ;)).
    */
    #ifdef ZNSOCK_USE_POLL
    struct pollfd fd = {
      .fd = socket,
      .events = POLLIN
    };

    int ret = poll(&fd, 1, ZNSOCK_CLOSE_TIMEOUT);
    if (ret == 1)
      /* Make sure peer received everything. */
      recv(socket, NULL, 0, 0);
    #else /* ZNSOCK_USE_SELECT */
    fd_set set;
    FD_ZERO(&set);

    FD_SET(socket, &set);

    struct timeval tv;
    tv.tv_sec = ZNSOCK_CLOSE_TIMEOUT / 1000;
    tv.tv_usec = (ZNSOCK_CLOSE_TIMEOUT % 1000) * 1000;

    int ret = select(1, &set, NULL, NULL, &tv);
    if (ret == 1)
      /* Make sure peer received everything. */
      recv(socket, NULL, 0, 0);
    #endif
  }
  /* Close socket. */
  close(socket);
}
