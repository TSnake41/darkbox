#ifndef H_PARSE_ARGS
#define H_PARSE_ARGS

#include <stdbool.h>

typedef struct choice_args {
    char *keys,
         *prompt;

    unsigned char default_key;
    int timeout;

    bool display_choices;
    bool case_sensitive;

    bool display_help;
} choice_args;

bool parse_args(int argc, char **argv, choice_args *args);

#endif /* H_PARSE_ARGS */
