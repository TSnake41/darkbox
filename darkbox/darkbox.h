/*
    Darkbox - A Fast and Portable Console IO Server
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

#ifndef H_DARKBOX
#define H_DARKBOX

#include <stdlib.h>

/* --- Beginning of user configuration --- */

/* Maximal size of texts */
#define MAX_TEXT_LENGTH 1024

/* Maximal size of command (including command prefix) */
#define MAX_COMMAND_SIZE 128

/* Prefix of any command */
#define COMMAND_START_CHR  '-'
#define COMMAND_START_CHR2 '/'

/* --- End of user configuration --- */

typedef struct darkbox_cmd {
    char *cmd;
    int count;
    /* no argv nor argc because they are parsed
       from stdin to get the parsing more responsive
    */
} darkbox_cmd;

void start(void);

void execute_line(void);
bool parse_cmd(darkbox_cmd *command);
void execute_cmd(darkbox_cmd cmd);

void input_server(void);

int read_int(void);
void read_string(char *buffer, const size_t max_length);

#endif /* H_DARKBOX */
