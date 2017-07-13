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

/* Network Message System, a simple way to send framed data. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <socket.h>

#define nms_flags socket_default_flags

bool nms_send(socket_int socket, void *data, uint16_t length)
{
    /* Convert length to network bytes */
    uint8_t length_bytes[2] = {
    	length & 0xFF,
    	length >> 8,
    };

    if (send(socket, length_bytes, 2, nms_flags) == -1 ||
        send(socket, data, length, nms_flags) == -1)
        /* Unable to send data. */
        return true;

    return false;
}

bool nms_recv(socket_int socket, void **buffer, uint16_t *received)
{
    uint8_t head_bytes[2];

    if (recv(socket, head_bytes, 2, nms_flags) == -1)
        return true;

    /* Convert message length network bytes to integer. */
    *received = head_bytes[0] + (head_bytes[1] << 8);

    *buffer = malloc(*received);
    if (*buffer == NULL) {
        /* Out of memory ? */
        free(*buffer);
        return true;
    }

    if (recv(socket, *buffer, *received, nms_flags) == -1) {
        /* Failed ? */
        free(*buffer);
        return true;
    }

    return false;
}

bool nms_recv_no_alloc(socket_int socket, void *buffer, uint16_t *received)
{
    uint8_t head_bytes[2];

    if (recv(socket, head_bytes, 2, nms_flags) == -1)
        return true;

    /* Convert message length network bytes to integer. */
    *received = head_bytes[0] + (head_bytes[1] << 8);

    if (recv(socket, buffer, *received, nms_flags) == -1)
        /* Failed ? */
        return true;

    return false;
}
