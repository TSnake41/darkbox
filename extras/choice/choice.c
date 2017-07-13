#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <core.h>
#include <core_i.h>

#include "strcasechr.h"

#include "parse_args.h"
#include "choice.h"

#define s_strchr(str, c, sensitive) \
	(sensitive ? strchr(str, c) : strcasechr(str, c))

#define WAIT_DELAY ((int)(WAIT_FREQUENCY * 1000))

unsigned char run_choice(choice_args args);
bool check_key(choice_args args, unsigned char *index);

int main(int argc, char **argv)
{
    choice_args args;

    if (parse_args(argc, argv, &args))
        return 0xFF; /* 0xFF : Choice exit code */

    if (args.display_help)
        goto show_help;

    return run_choice(args);

    show_help:
        puts("choice - Lightweight and Portable choice implementation - Astie Teddy (TSnake41)\n"
             "Usage : choice [/C[:]choices] [/T[:]timeout /D[:]default_key]\n"
             "               [/M[:]prompt] [/N] [/CS]\n\n"
             " - C: Define choices\n"
             " - T: Define timeout (need D)\n"
             " - D: Define default key (need T)\n"
             " - M: Define prompt text.\n"
             " - N: Do not display choices and '?'\n"
             " - CS: Case-sensitive mode\n\n"
             "NOTE: '-' can be used in place of '/'");
        return 0;
}

unsigned char run_choice(choice_args args)
{
    unsigned char key_index,
                  default_key_index;

    if (args.default_key != 0)
        default_key_index = strchr(args.keys, args.default_key) - args.keys + 1;

    /* Display prompt */
    if (args.prompt != NULL) {
        fputs(args.prompt, stdout);
        /* Add following space. */
        putchar(' ');
    }

    if (args.display_choices) {
        /* Display key list */
        const char *keys = args.keys;

        putchar('[');
        /* Display first key */
        putchar(*keys);

        /* Display other keys */
        while (*++keys) {
            if (!isprint(*keys))
                /* Don't display garbage ! */
                continue;

            putchar(',');
            putchar(*keys);
        }

        putchar(']');
        putchar('?');
    }

    fflush(stdout);

    if (args.default_key != 0) {
        /* Use delay */
        unsigned int wait_cout = (args.timeout * 1000) / WAIT_DELAY;

        while (wait_cout--) {
            if (core_peek_stdin()) {
                if (check_key(args, &key_index))
                    putchar('\a');
                else
                    return key_index;
            } else
                core_sleep(WAIT_DELAY);
        }

        return default_key_index;
    } else
        while (true)
            if (check_key(args, &key_index))
                putchar('\a');
            else
                return key_index;

    /* Never archived */
    return 0;
}

bool check_key(choice_args args, unsigned char *index)
{
    int key = core_is_stdin_console() ? core_getkey() : getchar();

    if (key == EOF) {
        /* Returns 0 */
        *index = 0;
        return false;
    }

    char *chr_ptr = s_strchr(args.keys, key, args.case_sensitive);

    if (chr_ptr == NULL)
        return true;

    *index = chr_ptr - args.keys + 1;

    #ifndef WIN32
    if (args.timeout == 0)
    #endif
        putchar(*chr_ptr);

    return false;
}
