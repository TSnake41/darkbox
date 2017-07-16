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

#ifndef H_NMS
#define H_NMS

#include <stdbool.h>
#include <stdint.h>

#include "socket.h"

/**
 * Send a Network Message to 'socket'.
 * Returns true on fail.
 */
bool nms_send(socket_int socket, void *buffer, uint16_t length);

/**
 * Recieve a Network Message from 'socket'.
 * NOTE: You need to manually free buffer.
 * Returns true on fail.
 */
bool nms_recv(socket_int socket, void **buffer, uint16_t *length);

/**
 * Recieve a Network Message from 'socket'.
 * NOTE: Size of buffer must be at least of 2^16-1 bytes.
 * Returns true on fail.
 */
bool nms_recv_no_alloc(socket_int socket, void *buffer, uint16_t *received);

#endif /* H_NMS */
