/*
  ptnio - Portable TCP Network IO
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

#ifndef H_MESSAGE
#define H_MESSAGE

#include <znsock.h>

typedef struct socket_message {
  unsigned int argc;
  char **argv;
} socket_message;

/**
 * Recieve message from s and store it to
 * 'message', allocate recieved arguments
 * (which need to be freed using message_free())
 */
bool message_recv(znsock socket, socket_message *message);

/**
 * Send a message to s.
 */
bool message_send(znsock socket, socket_message message);

/**
 * Free all variables of message allocated by message_recv.
 */
void message_free(socket_message message);

#endif /* H_MESSAGE */
