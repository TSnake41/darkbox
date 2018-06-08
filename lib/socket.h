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

#ifndef H_SOCKETS
#define H_SOCKETS

#include <stdbool.h>

/* Add cross-platform sockets */
#ifndef WIN32

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 720
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

typedef int socket_int;

#define socket_default_flags (MSG_NOSIGNAL)
#define socket_is_valid(socket) ((socket) != -1)
#else
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_int;
typedef unsigned short in_port_t;

#define socket_default_flags (0)
#define close closesocket
#define inet_pton inet_pton_win
#define inet_ntop inet_ntop_win
#define poll WSAPoll

/* Avoid pointer warnings (since WinAPI uses char * instead of void *). */
#ifdef AVOID_POINTER_WARNING
#define send(s, b, l, f) send(s, (char *)(b), l, f)
#define recv(s, b, l, f) recv(s, (char *)(b), l, f)
#define setsockopt(s, l, n, v, ol) setsockopt(s, l, n, (char *)(v), ol)
#define getsockopt(s, l, n, v, ol) getsockopt(s, l, n, (char *)(v), ol)
#endif

#define socket_is_valid(socket) ((socket) != INVALID_SOCKET)
#endif

void socket_init(void);
void socket_end(void);
int socket_available(socket_int);
bool socket_set_blocking(socket_int socket, bool blocking);
bool socket_set_read_timeout(socket_int socket, long timeout);

#ifdef WIN32
int inet_pton_win(int af, const char *src, void *dst);
const char *inet_ntop_win(int af, const void *src, char *dst, socklen_t size);
#endif

#endif /* H_SOCKETS */
