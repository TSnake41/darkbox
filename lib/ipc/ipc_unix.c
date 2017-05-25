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
#include <string.h>
#include <fcntl.h>
#include <sys/un.h>

#include <ipc.h>

static ipc_socket_t ipc_create_socket(const char *id, struct sockaddr_un *saun_o)
{
    const char *f_path_prefix = ".socket_";
    strcpy(saun_o->sun_path, f_path_prefix);
    strcat(saun_o->sun_path, id);

    saun_o->sun_family = AF_UNIX;

    if ((strlen(id) + strlen(f_path_prefix) + 1) > 108) {
        fputs("IPC: ID too long !\n", stderr);
        return -1;
    }

    socket_t s;
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return -1;

    return s;
}

ipc_socket_t ipc_server_new(const char *id, int max_pending)
{
    socket_t s;
    struct sockaddr_un saun;
    if ((s = ipc_create_socket(id, &saun)) == 0)
        return -1;

    socklen_t l = sizeof(saun.sun_family) + strlen(saun.sun_path);

    unlink(saun.sun_path);

    if (bind(s, (const struct sockaddr *)&saun, l) < 0) {
        close(s);
        return -1;
    }

    listen(s, max_pending);
    return s;
}

ipc_socket_t ipc_client_new(char *id)
{
    socket_t s;
    struct sockaddr_un saun;

    if ((s = ipc_create_socket(id, &saun)) == -1)
        return -1;

    if (connect(s, (struct sockaddr *)&saun, sizeof(struct sockaddr_un)) == -1) {
        close(s);
        return -1;
    }

    return s;
}
