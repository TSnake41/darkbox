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

#include <core.h>
#include <core_i.h>

#include "parser.h"

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

int run_menu(menu_t menu);
void render_menu(menu_t menu);

int main(int argc, char const *argv[])
{
    core_init();

    if (argc < 2)
        goto showHelp;

    menu_t menu = parse(argc - 1, argv + 1);
    llist_t *items = menu.items;

    puts("Menu: ");
    printf("  cc: %d\n", menu.current_choice);

    while (items) {
        item_t *item = items->value;
        puts("  Item: ");

        printf("    a: %d\n", item->a);
        printf("    bc: %X\n", item->bc);
        printf("    bfc: %X\n", item->bfc);
        printf("    fc: %X\n", item->fc);
        printf("    jump: %d\n", item->jump);
        printf("    l: %d\n", item->length);
        printf("    p: %d %d\n", item->pos_x, item->pos_y);

        char *text = malloc(item->str.l + 1);
        text[item->str.l] = '\0';
        memcpy(text, item->str.str_p, item->str.l);

        printf("    t: %s\n", text);
        items = items->next;
    }

    return 0;

    showHelp:
        puts("makemenu - Darkbox extension to make menus - Astie Teddy (TSnake41)\n"
             "Syntax : \n"
			 "  makemenu -key:value -keyN:valueN [...]\n"
             "  NOTE: This mode need both darkbox output and input server\n"
             "        context otherwise, you need to use this syntax :\n\n"
             "        darkbox -i | makmenu -key:value -keyN:valueN | darkbox\n\n"
             " Available keys : \n"
             "   a: Alignements: Right, Left, Center\n"
             "   fc: Foreground color\n"
             "   ffc: Foreground focused color\n"
             "   bc: Background color\n"
             "   bfc: Background focused color\n"
             "   j: Jump (how many lines to skip after\n"
             "   l: Line length\n"
             "   t: Text (-t:\"Example\"), act as an item separator\n"
             "   c: Current choice (global)\n"
             "   p: Position (format: x;y)\n\n");
        return 0;
}

int run_menu(menu_t menu)
{

}

void render_menu(menu_t menu)
{

}
