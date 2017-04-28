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
#include <string.h>
#include <ctype.h>

#include <fllist.h>

#include "parser.h"

const c_str_t empty = { .l = 0 };

menu_t parse(int i_count, const char **i_values)
{
    menu_t menu;
    item_t ci, *new_item;
    bool next = false;

    menu.items = NULL;
    menu.current_choice = DEFAULT_CC;

    /* Default values */
    ci.a       =   DEFAULT_A;
    ci.jump    =   DEFAULT_JUMP;
    ci.length  =   DEFAULT_LENGTH;
    ci.bc      =   DEFAULT_BC;
    ci.bfc     =   DEFAULT_BFC;
    ci.fc      =   DEFAULT_FC;
    ci.ffc     =   DEFAULT_FFC;

    ci.pos_x = 0;
    ci.pos_y = 0;

    ci.str = empty;

    for (int i = 0; i < i_count; i++) {
        if (!read_item(i_values[i], &menu, &ci, &next))
            fprintf(stderr, "ERROR: Parsing error on %s !\n", i_values[i]);

        if (next) {
            /* Calculate position with alignement */
            switch (ci.a) {
                case CENTER:

                    break;

                case RIGHT_ALIGN: ;
                    int old_pos_x = ci.pos_x;
                    ci.pos_x += ci.length - ci.str.l;

                    /* Do */
                    if (ci.pos_x < old_pos_x)
                    ;

                    break;
            }

            /* Clone ci to new_item */
            new_item = malloc(sizeof(item_t));
            memcpy(new_item, &ci, sizeof(item_t));

            menu.items = queue_push(menu.items, new_item);
            next = false;

            ci.pos_y += ci.jump;
        }
    }

    return menu;
}

void free_menu(menu_t menu)
{
    /* Clear items */
    if (menu.items)
        do
            free(menu.items->value);
        while ((menu.items = queue_pop(menu.items)));
}

bool read_item(const char *exp, menu_t *menu, item_t *item, bool *is_t)
{
    if (exp[0] != '-' && exp[0] != '/')
        return 0;

    const char *separator = strchr(exp, ':');

    if (separator == NULL)
        /* No separator ? */
        return 0;

    c_str_t str;
    const char *key = exp + 1;
    const char *value = separator + 1;

    str.l = strlen(value);

    /* Handle quote-based values */
    if (*value == '"') {
        value++;
        /* Remove quotes */
        str.l -= 2;
    }

    str.str_p = value;

    switch (tolower(*key)) {
        case 'a':
            switch (tolower(*value)) {
                case 'r': item->a = RIGHT_ALIGN; break;
                case 'l': item->a = LEFT_ALIGN; break;
                case 'c': item->a = CENTER; break;
            }
            break;

        case 'f':
            if (tolower(key[1]) == 'f')
                item->ffc = strtoul(value, NULL, 16);
            else
                item->fc = strtoul(value, NULL, 16);
            break;

        case 'b':
            if (tolower(key[1]) == 'f')
                item->bfc = strtoul(value, NULL, 16);
            else
                item->bc = strtoul(value, NULL, 16);
            break;

        case 'j':
            item->jump = strtoul(value, NULL, 10);
            break;

        case 'l':
            item->length = strtoul(value, NULL, 10);
            break;

        case 't':
            item->str = str;
            *is_t = true;
            break;

        case 'p':
            separator = strchr(value, ';');

            if (separator == NULL)
                return 0;

            item->pos_x = strtoul(value, NULL, 10);
            item->pos_y = strtoul(separator + 1, NULL, 10);
            break;

        case 'c':
            menu->current_choice = strtoul(value, NULL, 10);
            break;

        default:
            return 0;
    }

    return 1;
}
