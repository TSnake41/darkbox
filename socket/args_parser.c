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
#include <stdbool.h>

#include "args_parser.h"

bool parse_args(char **argv, int argc, socket_args *args)
{
    bool context_defined = false;
    memset(args, '\0', sizeof(socket_args));

    int i = 0;
    while (i < argc) {
        if (argv[i][0] != '-') {
            /* Invalid argument */
            i++;
            continue;
        }

        char *s = argv[i] + 1,
             *v = strchr(s, ':');

        switch (tolower(*s)) {
            case 'i':
                if (v != NULL)
                    args->id = v + 1;
                else {
                    fputs("ERROR: ID defined without value.\n", stderr);
                    return true;
                }
                break;

            case 'n':
                context_defined = true;
                args->new_instance = true;
                args->data.server.thread_count = 1;
                break;

            case 't':
                if (!args->new_instance) {
                    fputs("ERROR: Attempted to define 't' in non-server context.\n", stderr);
                    return true;
                }

				if (v != NULL)
					args->data.server.thread_count = strtol(v + 1, NULL, 0);
				else {
					fputs("ERROR: 't' defined without value.\n", stderr);
					return true;
				}
                break;

            case 'c':
                context_defined = true;
                client_request_data *client_data = &(args->data.client);
                i++;

                /* Begin client command list */
                int c_argc = client_data->command_argc = argc - i;
                client_data->command_argv = calloc(c_argc, sizeof(char *));

                int j = 0;
                while (j < c_argc) {
                    client_data->command_argv[j] = argv[i + j];
                    j++;
                }

                goto skip_loop;
        }

        i++;
    }

    skip_loop:
        if (args->id == NULL) {
            fputs("ERROR: ID not defined.\n", stderr);
            return true;
        }

        if (!context_defined) {
            fputs("INFO: No context defined.\n", stderr);
            return true;
        }

        return false;
}

void free_args(socket_args args)
{
    if (!args.new_instance && args.data.client.command_argv != NULL)
        free(args.data.client.command_argv);
}
