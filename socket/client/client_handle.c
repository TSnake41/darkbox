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

#ifndef WIN32
#define _XOPEN_SOURCE 700
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <nms.h>
#include <socket.h>

#ifndef WIN32
#include <core_i.h>
#endif

#ifdef WIN32
#include <windows.h>
int _setmode(int, int);
#define set_bin_mode(f) _setmode(_fileno(f), 0x8000)
#endif

#include "client_utils.h"
#include "client_handle.h"
#include "../cmd_codes.h"

#define client_handle_nms_send client_handle_send

const client_handle client_handles[] = {
    { client_handle_code, "new" },
    { client_handle_code, "free" },
    { client_handle_nms_recv, "accept" },
    { client_handle_code, "bind" },
    { client_handle_code, "listen" },
    { client_handle_recv, "recv" },
    { client_handle_send, "send" },
    { client_handle_code, "connect" },
    { client_handle_recv, "list" },
    { client_handle_nms_recv, "info" },
    { client_handle_poll, "poll" },
    { client_handle_nms_recv, "nms_recv" },
    { client_handle_nms_send, "nms_send" },
};
const unsigned int client_handles_count = sizeof(client_handles) / sizeof(*client_handles);

/** Recieve code from socket, and use it as exit code. */
void client_handle_code(socket_int socket)
{
    uint8_t code = recv_code(socket);
    exit(code);
}

/* First recieve code from IPC socket, then (if sucess)
 * read message write them to stdout until count!=0xFFFF
 */
void client_handle_recv(socket_int socket)
{
    uint8_t code = recv_code(socket);
    if (code != CMD_SUCCESS)
        /* Is an error. */
        exit(code);

    #ifdef WIN32
	/* Set stdout to binary mode */
    set_bin_mode(stdout);
    #endif

    uint16_t count = 0;
    char *buffer = malloc(0xFFFF);
    if (buffer == NULL)
        exit(CMD_CLIENT_OOM);

    do {
        if (nms_recv_no_alloc(socket, buffer, &count))
            exit(CMD_IPC_ERROR);

        fwrite(buffer, count, 1, stdout);
    } while(count != 0);

    free(buffer);
    exit(CMD_SUCCESS);
}

/** Send to IPC socket data from stdin. */
void client_handle_send(socket_int socket)
{
    setvbuf(stdin, NULL, _IONBF, 0);

    #ifdef WIN32
	/* Set stdin to binary mode */
    set_bin_mode(stdin);
    #endif

    uint16_t count = 0;
    char *buffer = malloc(0xFFFF);
    if (buffer == NULL)
        exit(CMD_CLIENT_OOM);

    do {
        count = fread(buffer, 1, 0xFFFF, stdin);

        if (nms_send(socket, buffer, count))
            exit(CMD_IPC_ERROR);
    } while(count == 0xFFFF);

    free(buffer);

    uint8_t code = recv_code(socket);
    exit(code);
}

/** Recieve code from IPC socket, then (if success)
 *  recieve network message and write it to stdout.
 */
void client_handle_nms_recv(socket_int socket)
{
    uint8_t code = recv_code(socket);
    if (code != CMD_SUCCESS)
        /* Is an error. */
        exit(code);

    #ifdef WIN32
    /* Set stdout to binary mode */
    set_bin_mode(stdout);
    #endif

    uint16_t recieved = 0;
    void *buffer;

    if (nms_recv(socket, &buffer, &recieved))
        exit(CMD_IPC_ERROR);
    else
        fwrite(buffer, 1, recieved, stdout);

    free(buffer);
    exit(recieved ? CMD_SUCCESS : CMD_NMS_ZERO_SIZE);
}

/** Implementation needed */
void client_handle_poll(socket_int socket)
{
    /* TODO */
}
