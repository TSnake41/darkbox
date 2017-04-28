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
#define _XOPEN_SOURCE 720
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>

typedef int socket_t;

#define socket_default_flags (MSG_NOSIGNAL)
#else
#include <winsock2.h>
#include <ws2tcpip.h>
typedef SOCKET socket_t;
typedef unsigned short in_port_t;

#define socket_default_flags (0)
#define close closesocket
#endif

void socket_init(void);
void socket_end(void);
int available(socket_t);
bool set_blocking(socket_t socket, bool blocking);

#endif /* H_SOCKETS */
