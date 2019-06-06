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
/* TODO: Possibility to use a timeout. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include <znsock.h>

bool nms_send(znsock socket, const void *data, uint16_t length)
{
  /* Convert length to network bytes */
  uint8_t length_bytes[2] = {
  	length & 0xFF,
  	length >> 8,
  };

  if (znsock_send(socket, length_bytes, 2) == -1)
    return true;

  if (length > 0)
    if (znsock_send(socket, data, length) == -1)
      /* Unable to send data. */
      return true;

  return false;
}

bool nms_recv(znsock socket, void **buffer, uint16_t *received)
{
  uint8_t head_bytes[2];

  if (znsock_recv(socket, head_bytes, 2) == -1)
    return true;

  /* Convert message length network bytes to integer. */
  *received = head_bytes[0] + (head_bytes[1] << 8);

  if (*received == 0)
    /* Handle zero-size request. */
    return false;

  *buffer = malloc(*received);
  if (*buffer == NULL)
    /* Out of memory ? */
    return true;

  int ret = znsock_recv(socket, *buffer, *received);

  if (ret != *received) {
    /* Failed ? */
    free(*buffer);
    return true;
  }

  return false;
}

bool nms_recv_no_alloc(znsock socket, void *buffer, uint16_t *received)
{
  uint8_t head_bytes[2];

  if (znsock_recv(socket, head_bytes, 2) != 2)
    return true;

  /* Convert message length network bytes to integer. */
  *received = head_bytes[0] + (head_bytes[1] << 8);

  if (*received == 0)
    /* zero-size request */
    return false;

  int ret = znsock_recv(socket, buffer, *received);

  if (ret != *received)
    /* Failed ? */
    return true;

  return false;
}
