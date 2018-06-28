/*
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

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

/* TODO: Clean this ! */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <socket.h>

#ifndef WIN32
#include <sys/time.h>
#include <fcntl.h>
#else
#include <windows.h>
#endif

#ifndef GRACEFUL_CLOSE_TIMEOUT
#define GRACEFUL_CLOSE_TIMEOUT 20000
#endif

#if (!defined(WIN32) || defined(FORCE_POLL)) && !defined(FORCE_SELECT)
#define USE_POLL
#else
#define USE_SELECT
#endif

void socket_init(void)
{
  #ifdef WIN32
  WSADATA wsa;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
    fputs("WSAStartup failed !", stderr);
    exit(-1);
  }
  #endif
}

void socket_end(void)
{
  #ifdef WIN32
  WSACleanup();
  #endif
}

bool socket_set_read_timeout(socket_int socket, long timeout)
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

bool socket_set_blocking(socket_int socket, bool blocking)
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
  return !ioctlsocket(socket, FIONBIO, &mode);
  #endif
}

void socket_graceful_close(socket_int socket)
{
  if (shutdown(socket, SHUT_RDWR) == 0) {
    /* Okay, we shutdowned socket, now, we have to wait until all on-fly
       data are received by pair, however, we must make sure that this check
       does not idle eternally using a timeout (with polling ;)).
    */
    #ifdef USE_POLL
    struct pollfd fd = {
      .fd = socket,
      .events = POLLIN
    };

    int ret = poll(&fd, 1, GRACEFUL_CLOSE_TIMEOUT);
    if (ret == 1)
      /* Make sure peer received everything. */
      recv(socket, NULL, 0, 0);
    #else /* USE_SELECTs */
    fd_set set;
    FD_ZERO(&set);

    FD_SET(socket, &set);

    struct timeval tv;
    tv.tv_sec = GRACEFUL_CLOSE_TIMEOUT / 1000;
    tv.tv_usec = (GRACEFUL_CLOSE_TIMEOUT % 1000) * 1000;

    int ret = select(1, &set, NULL, NULL, &tv);
    if (ret == 1)
      /* Make sure peer received everything. */
      recv(socket, NULL, 0, 0);
    #endif
  }
  /* Either the socket is not connected or something went wrong, just close it. */
  close(socket);
}

#ifdef WIN32
/* http://stackoverflow.com/a/20816961 */
int inet_pton_win(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN + 1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN + 1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0)
    switch(af) {
      case AF_INET:
        *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
        return 1;

      case AF_INET6:
        *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
        return 1;
    }

  return 0;
}

const char *inet_ntop_win(int af, const void *src, char *dst, socklen_t size)
{
  struct sockaddr_storage ss;
  unsigned long s = size;

  ZeroMemory(&ss, sizeof(ss));
  ss.ss_family = af;

  switch(af) {
    case AF_INET:
      ((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src;
      break;
    case AF_INET6:
      ((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src;
      break;
    default:
      return NULL;
  }
  /* cannot direclty use &size because of strict aliasing rules */
  return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst, &s) == 0)?
          dst : NULL;
}
#endif
