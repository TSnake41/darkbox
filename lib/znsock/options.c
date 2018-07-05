#include <znsock.h>

bool znsock_set_read_timeout(znsock socket, long timeout)
{
  #ifndef WIN32
  struct timeval t = {
    .tv_usec = (timeout % 1000) * 1000,
    .tv_sec = timeout / 1000
  };
  #else
  DWORD t = (DWORD)timeout;
  #endif

  return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t)) == -1;
}

bool znsock_set_keepalive(znsock socket, int keepalive)
{
  return setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(int)) != 0;
}

bool znsock_set_blocking(znsock socket, bool blocking)
{
  #ifndef WIN32
  int flags = fcntl(socket, F_GETFL, 0);
  if (flags == -1)
    return true;

  /* Clear the blocking flag. */
  flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
  return fcntl(socket, F_SETFL, flags) == -1;
  #else
  unsigned long mode = !blocking;
  return ioctlsocket(socket, FIONBIO, &mode);
  #endif
}
