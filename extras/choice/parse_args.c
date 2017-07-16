/*
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
#include <string.h>
#include <ctype.h>

#include "strcasechr.h"

#include "choice.h"
#include "parse_args.h"

#define s_strchr(str, c, sensitive) \
	(sensitive ? strchr(str, c) : strcasechr(str, c))

static char **get_value(char **argv, char **value);

bool parse_args(int argc, char **argv, choice_args *args)
{
    args->keys = DEFAULT_KEYS;
    args->prompt = NULL;

    args->default_key = 0;
    args->timeout = 0;

    args->display_choices = true;
    args->case_sensitive = false;

    args->display_help = false;

    bool timeout_defined = false;

    while (*(++argv)) {
        char *current_arg = *argv;

        if (*current_arg != '/' && *current_arg != '-') {
            /* Not valid,*/
            fputs(ERROR_UNEXPECTED_ARG, stderr);
			fputs(current_arg, stderr);
			fputc('\n', stderr);
            return true;
        }

        switch (tolower(current_arg[1])) {
            case 'c':
                if (tolower(current_arg[2]) == 's') {
                    args->case_sensitive = true;
                    break;
                }

                argv = get_value(argv, &args->keys);
                break;

            case 't': ;
                char *n = "";
                argv = get_value(argv, &n);

                char *t_end_ptr;
                args->timeout = strtoul(n, &t_end_ptr, 0);

                if (t_end_ptr == n) {
                    fputs(ERROR_UNEXPECTED_VALUE, stderr);
					fputs(n, stderr);
					fputc('\n', stderr);
                    return true;
                }
                timeout_defined = true;
                break;

            case 'd': ;
                char *d_keys = "";
                argv = get_value(argv, &d_keys);

				if (*d_keys)
					/* Assume there is only one key. */
					args->default_key = *d_keys;
                break;

            case 'm':
                argv = get_value(argv, &args->prompt);
                break;

            case 'n':
                args->display_choices = false;
                break;

            case '?':
                args->display_help = true;
                return false;
        }
    }

    /* Late error checking */
    if (args->keys[0] == '\0') {
        fputs(ERROR_NO_KEY, stderr);
        return true;
    }

	if (args->default_key != 0
		&& s_strchr(args->keys, args->default_key, args->case_sensitive) == NULL) {

		fputs(ERROR_DEFAULT_KEY_NOT_FOUND, stderr);
		return true;
	}

    if (timeout_defined && args->default_key == 0) {
        fputs(ERROR_DEFAULT_KEY_MISSING, stderr);
        return true;
    } else if (!timeout_defined && args->default_key != 0) {
        fputs(ERROR_TIMEOUT_MISSING, stderr);
        return true;
    }

    return false;
}

static char **get_value(char **argv, char **value)
{
    char *separator = strchr(*argv, ':');

	/* Avoid crash by not defining a value that doesn't exists. */
	if (separator != NULL || argv[1] != NULL)
		*value = separator == NULL ? *++argv : separator + 1;

    return argv;
}
