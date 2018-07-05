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
    if (send(socket, data, length, MSG_WAITALL) == -1)
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
