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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#endif

#include <core.h>
#include <core_i.h>

#include "darkbox.h"
#include "sprite.h"

char text_buffer[MAX_TEXT_LENGTH],
     command_buffer[MAX_COMMAND_SIZE];

int origin_x, origin_y;

int main(int argc, char const *argv[])
{
    if (argc > 1 && (argv[1][0] == '-' || argv[1][0] == '/'))

        switch (argv[1][1]) {

            /* Get help */
            case '?':
                goto showHelp;

            /* Start input server */
            case 'i':
                input_server();
                return 0;

            /* Returns a non-zero value if data is
               available in stdin otherwise, return 0
            */
            case 'k': ;
                #ifndef WIN32
                return core_kbhit();
                #else
                /* Unfortunately, Windows's kbhit take input only from Console and
                   not stdin, so, we must hack to makes this working with stdin.
                */
                HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
                DWORD available = 0;
                PeekNamedPipe(hin, NULL, 0, NULL, &available, NULL);
                return available;
                #endif

            case 'w': ;
                if (argc < 2)
                    return 1;

                sleep_ms(strtol(argv[2], NULL, 10));
                return 0;
        }

    /* Initialize Console IO */
    core_init();

    start();

	/* Never reached */
    return 0;

    showHelp:
        puts("darkbox - Fast Portable Console IO Server - Astie Teddy (TSnake41)\n"
             "Syntaxes : \n"
             "  1: (code) | darkbox\n"
             "  2: darkbox -i | (code)\n"
             "  3: darkbox -w t\n"
             "  4: darkbox -k\n\n"
             " 1: Start darkbox as output server\n"
             " 2: Start darkbox as input server\n"
             " 3: Wait t ms\n"
             " 4: Return a non-nul value if data is avaialble in stdin.\n\n"
             "NOTE: darkbox support both '-' and '/' as command prefixes.\n"
             "For more information, see README or http://batch.xoo.it/p41497.htm\n");
        return 0;
}

void start(void)
{
    while (true)
        execute_line();
}

/* Parse a line from stdin and execute it, if this is
   not a command, print the current line in stdout
*/
void execute_line(void)
{
    command_t cmd;

    if (parse_cmd(&cmd))
        execute_cmd(cmd);
}

/* Parse the command from stdin if this is not
   a command, print the current line to stdout
*/
bool parse_cmd(command_t *command)
{
    char *str = command_buffer;

    read_string(str, MAX_COMMAND_SIZE);

    if (*str != COMMAND_START_CHR &&
        *str != COMMAND_START_CHR2)
        goto no_command;

    str++;

    /* use the count if specified in command */
    command->count = isdigit(*str)
        ? strtol(str, &str, 10) : 1;

    if (isalpha(*str)) {
        /* define literals */
        command->cmd = str;

        /* check literals */
        while (*str) {
            if (!isalpha(*str)) {
                fputs("Parsing error: Invalid litterals", stderr);
                return false;
            }
            str++;
        }
    } else {
        fputs("Parsing error: No command specified", stderr);
        return false;
    }

    return true;

    no_command:
        fputs(str, stdout);
        putchar(' ');

        if (strchr(str, '\n') != 0)
            /* do not break next line */
            return false;

        char c = 0;
        while (c != '\n')
            putchar(c = getchar());

        return false;
}

/* Execute a command, get args from stdin */
void execute_cmd(command_t cmd)
{
    for (int i = 0; i < cmd.count; i++) {
        char *str = cmd.cmd;

        while (*str) {

            switch (tolower(*str)) {

                case 'a':
                    /* Syntax : -a Integer
                       Usage : Print the character
                       corresponding to the integer
                    */

                    putchar(read_int());
                    break;

                case 'd':
                    /* Syntax : -d Text
                       Usage : Display Text
                    */

                    read_string(text_buffer, MAX_TEXT_LENGTH);
                    fputs(text_buffer, stdout);
                    break;

                case 'n':
                    /* Syntax : -n
                       Usage : Go to the next line
                    */

                    putchar('\n');
                    break;

                case 'c':
                    /* Syntax : -c color
                       color is in hexadecimal format.

                       Usage : Change the current color
                               0xF0 : Background
                               0x0F : Foreground
                    */
                    core_change_color(read_int());
                    break;

                case 'g':
                    /* Syntax : -g x y
                       Usage : Move cursor to (x;y)
                    */

                    core_gotoxy(origin_x + read_int(), origin_y + read_int());
                    break;

                case 'h':
                    /* Syntax : -h 0/1
                       Usage : Hide or show cursor
                    */

                    core_change_cursor_state(read_int());
                    break;

                case 's':
                    /* Syntax : -s
                       Usage : Clear console
                    */

                    core_clear_console();
                    break;

                case 'o':
                    /* Syntax : -o x y
                       Usage : Change goto origin
                    */

                    origin_x = read_int();
                    origin_y = read_int();
                    break;

                case 'r':
                    /* Syntax : -r
                       Usage : Reset foreground and background colors
                    */

                    core_reset_color();
                    break;

                case 'q':
                    /* Syntax : -q
                       Usage : Properly stop darkbox
                    */

                    /* free_buffers(); */
                    exit(0);
                    break;

                case 'w':
                    /* Syntax : -w t
                       Usage : Wait t miliseconds
                    */

                    #ifndef WIN32
                    /* Flush stdout on *NIXes because they use
                       buffers. On DJGPP, this behavior works
                    */
                    fflush(stdout);
                    #endif
                    /* On Windows, we do not need to
                       because this is done automaticaly
                    */

                    sleep_ms(read_int());
                    break;

                case 'x': ;
                    /* Syntax : -x offset_x offset_y sprite_file
                       Usage : Draw 'sprite_file' at (offset_x;offset_y).
                    */

                    int offset_x = read_int(),
                        offset_y = read_int();

                    read_string(text_buffer, MAX_TEXT_LENGTH);

                    draw_sprite(offset_x, offset_y, text_buffer);
                    break;

                /*
                    case 'b': ;
                        Syntax : -b
                        Usage : Back mode; return stdin
                                until a line start with '-d' or '/d'

                        Deprecated: Replaced by -e in darkbox_t

                        type test.txt | darkbox_t -e
                */
            }

            str++;
        }
    }
}

/* Start input server */
void input_server(void)
{
    core_input_initialize(1);
    input_event_t e;

    while (!feof(stdin)) {
        core_input_get_event(&e);

        switch (e.type) {
            case KEY_PRESS:
                printf("k %d\n", e.event.key_press);
                break;

            case MOUSE:
                printf("m %d %d %d\n", e.event.mouse.x, e.event.mouse.y, e.event.mouse.b);
                break;
        }

        fflush(stdout);
    }

    core_input_terminate(1);
}
/* Read the next integer from stdin */
int read_int(void)
{
    char c;
    int pos = 0;

    /* skip beginning spaces */
    do
        c = getchar();
    while (isspace(c));

    while (!isspace(c) && pos < MAX_TEXT_LENGTH) {
        text_buffer[pos] = c;
        pos++;
        c = getchar();
    }
    text_buffer[pos++] = '\0';

    return strtol(text_buffer, NULL, 0);
}

/* Read the next argument from stdin */
void read_string(char *buffer, const size_t max_length)
{
    char c;
    int pos = 0;

    /* skip beginning spaces */
    do
        c = getchar();
    while (isspace(c));

    /* quote mode */
    bool quote = c == '"';

    if (quote)
        /* skip quote chracter */
        c = getchar();

    char *end_chars = quote ? "\"" : " \n\r\t";

    while (!strchr(end_chars, c) && (pos < max_length) ) {

        if (c == '\\')
            /* skip escape chracter, the next chracter is not interpreted */
            c = getchar();

        buffer[pos] = c;
        pos++;

        c = getchar();
    }

    buffer[pos++] = '\0';
}

#ifndef WIN32
static void sleep_ms(unsigned long ms)
{
    struct timespec req;
    time_t sec = (int)(ms / 1000);
    ms -= sec * 1000;
    req.tv_sec = sec;
    req.tv_nsec = ms * 1e+6L;
    while (nanosleep(&req, &req) == -1)
        continue;
}
#endif
