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

#ifndef H_PARSER
#define H_PARSER

#define MAX_TEXT_LENGTH 192

#define DEFAULT_A       LEFT_ALIGN
#define DEFAULT_JUMP    0
#define DEFAULT_LENGTH  24
#define DEFAULT_BC      0x0
#define DEFAULT_BFC     0x0
#define DEFAULT_FC      0x7
#define DEFAULT_FFC     0xF

#define DEFAULT_CC      1

#include <stdlib.h>
#include <fllist.h>

/* Constant string */
typedef struct c_str {
    int l;
    const char *str_p;
} c_str_t;

typedef struct Item {
    unsigned int a, length, jump, pos_x, pos_y;
    unsigned char fc, ffc, bc, bfc;
    c_str_t str;
} item_t;

typedef struct Menu {
    int current_choice; /* cc variable */
    queue_t *items; /* item queue */
} menu_t;

enum Alignement {
    LEFT_ALIGN = 0,
    RIGHT_ALIGN,
    CENTER
};

typedef unsigned int bool;
enum {
    false = 0, true = !false
};

menu_t parse(int i_count, char const **i_values);
bool read_item(const char *exp, menu_t *menu, item_t *item, bool *is_t);
void free_menu(menu_t menu);

#endif /* H_PARSER */
