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

/*
    Interprocess communication for SockeT.

    Implementation differ between POSIX and Windows :
     - POSIX uses Unix Domain sockets
     - Windows uses Named Pipes.
*/

#ifndef H_IPC
#define H_IPC

#include <stdbool.h>
#include <socket.h>

#define ipc_socket_is_valid(socket) ((socket) != -1)

#ifndef WIN32
typedef int ipc_socket_t;
#else
#include <windows.h>
typedef socket_t ipc_socket_t;
#endif

ipc_socket_t ipc_server_new(const char *id, int max_clients);
ipc_socket_t ipc_client_new(char *id);
ipc_socket_t ipc_server_accept(ipc_socket_t server);

#endif /* H_IPC */
