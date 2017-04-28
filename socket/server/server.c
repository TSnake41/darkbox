/*
    SockeT - Portable TCP and NMS Network IO interface.
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met :

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    * Neither the name of the name of Teddy Astie (TSnake41) nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY TEDDY ASTIE AND CONTRIBUTORS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL TEDDY ASTIE AND CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
    LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <fllist.h>
#include <sthread.h>
#include <socket.h>
#include <ipc.h>

#include "server.h"
#include "server_cmd.h"

#include "../args_parser.h"
#include "../message.h"
#include "../socket.h"

#ifndef WIN32
#include <strings.h>
#else
#define strcasecmp stricmp
#endif

static void server_thread(void *_server_socket);

void server(socket_args_t args)
{
    server_data_t data;
    smutex_new(&data.sock_list_mutex);
    data.args = &args;
    data.sock_list = NULL;

    data.ipc_server = ipc_server_new(args.id, 5);
    if (!ipc_socket_is_valid(data.ipc_server)) {
        fputs("ERROR: Unable to create a new IPC server.\n", stderr);
        return;
    }

    sthread_t threads[args.data.server.thread_count - 1];

    int i = 0;
    while (i < (args.data.server.thread_count - 1)) {
        if (sthread_new(&threads[i], server_thread, &data)) {
            fputs("ERROR: Unable to create a threads.\n", stderr);
            return;
        }
        i++;
    }

    /* Use main thread too. */
    server_thread(&data);
}

static void server_thread(void *_server_data)
{
    server_data_t *data = _server_data;

    while (true) {
        ipc_socket_t c = accept(data->ipc_server, NULL, NULL);

        message_t msg;

        if (message_recv(c, &msg))
            goto end;

        unsigned int i = 0;
        while (i < server_cmds_count) {
            if (stricmp(server_cmds[i].key, msg.argv[0]) == 0) {
                server_cmds[i].cmd(msg, c, data);
                break;
            }

            i++;
        }

        end:
            close(c);
    }
}
