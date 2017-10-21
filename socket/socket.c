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
#include <fllist.h>
#include <socket.h>

#include <tiny_assert.h>

/* #define NO_FORK */

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "client/client.h"
#include "server/server.h"
#include "args_parser.h"

#ifdef WIN32
static int fork_to_background_win(int argc, char **argv);
#endif

int main(int argc, char **argv)
{
    socket_init();

    /* Parse args */
    socket_args args;
    if (parse_args(argv, argc, &args))
        goto show_help;

    if (args.new_instance) {
        #ifndef NO_FORK
        /* Fork to background */
        #ifndef WIN32
        if (fork() != 0)
            exit(0);

        setsid();
        #else
        argc = fork_to_background_win(argc, argv);
        #endif
        #endif

        server(args);
	} else
        client(args);

    socket_end();
    return 0;

    show_help:
        puts("socket - Portable TCP and NMS Network IO interface - Astie Teddy (TSnake41)\n"
             "Syntaxes :\n"
             "  1: socket -id:ID -n [-t thread_count]\n"
             "  2: socket -id:ID -c command [command args]\n\n"
             "1: Start new socket server.\n"
             "2: Connect to a IPC server to perform a command.\n\n"
             "For more informations, see README at:\n"
             "https://gitlab.com/TSnake41/darkbox/tree/master/socket/README\n");
        return 0;
}

#ifdef WIN32
static int fork_to_background_win(int argc, char **argv)
{
    /* On Windows, no fork() = hell. */
    if (strcmp(argv[argc - 1], "-bg") != 0) {
        char cmdl[MAX_PATH + 5];
        memset(cmdl, '\0', MAX_PATH + 5);

        strcpy(cmdl, argv[0]);
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        memset(&si, '\0', sizeof si);

        strcpy(cmdl, argv[0]);

        int i = 1;
        while (i < argc) {
            const char space = ' ';
            strncat(cmdl, &space, 1);
            strcat(cmdl, argv[i]);
            i++;
        }
        strcat(cmdl, " -bg");

        if (CreateProcess(NULL, cmdl, NULL, NULL,
            false, 0, NULL, NULL, &si, &pi) == -1)
            fputs("ERROR: Unable to fork process\n", stderr);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        ExitProcess(0);
    }

    argv[argc - 1] = NULL;
    argc--;

    return argc;
}
#endif
