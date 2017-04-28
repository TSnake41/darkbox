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
#include <stdint.h>
#include <core.h>

enum {
    LITTLE_ENDIAN,
    BIG_ENDIAN
};

#ifndef WIN32
static int dump_get_endianess(void);
static uint32_t reverse_byte_order(uint32_t n);
#endif

void draw_sprite(char *sprite_file)
{
    int endianess;

    #ifndef WIN32
    endianess = dump_get_endianess();
    #else
    endianess = LITTLE_ENDIAN;
    #endif

    FILE *f = fopen(sprite_file, "rb");

    if (f == NULL)
        return;

    uint32_t count;
    fread(&count, sizeof(uint32_t), 1, f);

    #ifndef WIN32
    if (endianess == BIG_ENDIAN)
        /* Convert count to low endian */
        count = reverse_byte_order(count);
    #endif


}

#ifndef WIN32
static int dump_get_endianess(void)
{
    int test=1;
    char val;
    val = *((char*)&test);
    return val ? LITTLE_ENDIAN : BIG_ENDIAN;
}

static uint32_t reverse_byte_order(uint32_t n)
{
    uint8_t *bytes = (uint8_t *)&n;
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}
#endif
