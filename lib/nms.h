/*
  Copyright (C) 2017-2018 Teddy ASTIE

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/* Network Message System, a simple way to send framed data. */

#ifndef H_NMS
#define H_NMS

#include <stdbool.h>
#include <stdint.h>

#include "znsock.h"

/**
 * Send a Network Message to 'socket'.
 * Returns true on fail.
 */
bool nms_send(znsock socket, const void *buffer, uint16_t length);

/**
 * Recieve a Network Message from 'socket'.
 * NOTE: You need to manually free buffer.
 * Returns true on fail.
 */
bool nms_recv(znsock socket, void **buffer, uint16_t *length);

/**
 * Recieve a Network Message from 'socket'.
 * NOTE: Size of buffer must be at least of 2^16-1 bytes.
 * Returns true on fail.
 */
bool nms_recv_no_alloc(znsock socket, void *buffer, uint16_t *received);

#endif /* H_NMS */
