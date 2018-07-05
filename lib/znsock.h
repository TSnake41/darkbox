/*
    Copyright (c) 2018 Teddy ASTIE (TSnake41)

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

#ifndef H_ZNSOCK
#define H_ZNSOCK

#include <stdbool.h>

/* Add cross-platform sockets */
#ifndef WIN32

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 720
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

typedef int znsock;

#define znsock_is_valid(socket) ((socket) != -1)
#define znsock_no_signal (MSG_NOSIGNAL)
#else
#include <winsock2.h>
#include <ws2tcpip.h>

typedef SOCKET znsock;
typedef unsigned short in_port_t;

#define close closesocket
#define poll WSAPoll

#define SHUT_WR SD_SEND
#define SHUT_RD SD_RECEIVE
#define SHUT_RDWR SD_BOTH

/* Windows does not "really" support MSG_WAITALL. */
#define MSG_WAITALL 0

/* Avoid pointer warnings (since WinAPI uses char * instead of void *). */
#ifdef AVOID_POINTER_WARNING
#define send(s, b, l, f) send(s, (char *)(b), l, f)
#define recv(s, b, l, f) recv(s, (char *)(b), l, f)
#define setsockopt(s, l, n, v, ol) setsockopt(s, l, n, (char *)(v), ol)
#define getsockopt(s, l, n, v, ol) getsockopt(s, l, n, (char *)(v), ol)
#endif

#define znsock_is_valid(socket) ((socket) != INVALID_SOCKET)
#define znsock_no_signal (0)
#endif

bool znsock_init(void);
void znsock_cleanup(void);

bool znsock_set_blocking(znsock socket, bool blocking);
bool znsock_set_read_timeout(znsock socket, long timeout);
bool znsock_set_keepalive(znsock socket, int keepalive);
void znsock_close(znsock socket, bool graceful);

znsock znsock_ipc_server(const char *id, int max_pending);
znsock znsock_ipc_client(const char *id);
znsock znsock_ipc_accept(znsock s);
void znsock_ipc_close(znsock s);

#ifdef WIN32
int znsock_compat_inet_pton(int af, const char *src, void *dst);
const char *znsock_compat_inet_ntop(int af, const void *src, char *dst, socklen_t size);

#define inet_pton znsock_compat_inet_pton
#define inet_ntop znsock_compat_inet_ntop
#endif

#define znsock_recv_block(s, buffer, size, blocking) \
  recv(s, buffer, size, blocking ? MSG_WAITALL : 0)
#define znsock_send_block(s, buffer, size, blocking) \
  send(s, buffer, size, blocking ? MSG_WAITALL : 0)

#define znsock_recv(s, buffer, size) znsock_recv_block(s, buffer, size, true)
#define znsock_send(s, buffer, size) znsock_send_block(s, buffer, size, true)

#endif /* H_ZNSOCK */
