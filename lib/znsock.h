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

#ifndef H_ZNSOCK
#define H_ZNSOCK

#include <stddef.h>
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

int znsock_recv_block(znsock s, void *buffer, size_t size, bool blocking);

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

#define znsock_recv(s, buffer, size) znsock_recv_block(s, buffer, size, true)
#define znsock_send(s, buffer, size) send(s, buffer, size, 0)

#endif /* H_ZNSOCK */
