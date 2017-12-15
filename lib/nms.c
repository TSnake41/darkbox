/*
    Copyright (c) 2017 Teddy ASTIE (TSnake41)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/* Network Message System, a simple way to send framed data. */
/* TODO: Possibility to use a timeout. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <socket.h>

#define nms_flags socket_default_flags

bool nms_send(socket_int socket, const void *data, uint16_t length)
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
