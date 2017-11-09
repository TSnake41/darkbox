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
#include <socket_ipc.h>
#include <tiny_assert.h>

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

void server(socket_args args)
{
    server_data data;
    smutex_new(&data.pair_mutex);
    data.args = &args;
    data.pair_list = NULL;
    data.pair_count = 0;

    data.ipc_socket = socket_ipc_server_new(args.id, 5);
    if (!socket_is_valid(data.ipc_socket)) {
        fputs("ERROR: Unable to create the IPC server.\n", stderr);
        exit(1);
    }

    /* We consider the main thread (currently running) as a server thread. */
    size_t thread_count = args.data.server.thread_count - 1;

    sthread *threads = calloc(thread_count, sizeof(sthread));
    tiny_assert(threads == NULL); /* Check allocation. */

    int i = 0;
    while (i < thread_count) {
        tiny_assert(sthread_new(&threads[i], server_thread, &data));
        i++;
    }

    /* Consider the main thread as a server thread.. */
    server_thread(&data);

    /* Never reached (server_thread never returns) */
}

static void server_thread(void *_server_data)
{
    server_data *data = _server_data;

    while (true) {
        socket_int c = accept(data->ipc_socket, NULL, NULL);

        socket_message msg;

        if (message_recv(c, &msg)) {
            close(c);
            continue;
        }

        if (msg.argc != 0) {
            unsigned int i = 0;
            while (i < server_cmds_count) {
                if (stricmp(server_cmds[i].key, msg.argv[0]) == 0) {
                    server_cmds[i].cmd(msg, c, data);
                    break;
                }

                i++;
            }
        }

        close(c);
        message_free(msg);
    }
}
