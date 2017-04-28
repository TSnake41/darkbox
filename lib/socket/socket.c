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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <socket.h>

#ifndef WIN32
#include <fcntl.h>
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

int available(socket_t socket)
{
    int c;

    #ifndef WIN32
    int e = fcntl(socket, FIONREAD, &c);
    #else
    int e = ioctlsocket(socket, FIONREAD, &c);
    #endif

    return e != -1 ? c : -1;
}

bool set_blocking(socket_t socket, bool blocking)
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
