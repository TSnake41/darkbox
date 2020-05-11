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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <znsock.h>

#include "message.h"

/*
  Messages follow this format :
    uint arg_count

    uint arg1_size
    char arg1_data[arg1_size]
    [...]
    uint argN_size
    char argN_data[argN_size]
*/

bool message_recv(znsock socket, socket_message *message)
{
  if (message == NULL)
    goto error;

  memset(message, '\0', sizeof(socket_message));

  /* Recieve the argument count. */
  if (znsock_recv(socket, &message->argc, sizeof(unsigned int)) == -1)
    goto error;

  message->argv = calloc(message->argc, sizeof(char *));
  if (message->argv == NULL)
    goto error;

  unsigned int i = 0;
  while (i < message->argc) {
    /* Recieve the argument size (including NULL terminator). */
    unsigned int size;
    if (znsock_recv(socket, &size, sizeof(unsigned int)) == -1)
      goto error;

    /* Allocate memory for argument */
    message->argv[i] = malloc(size);
    if (message->argv[i] == NULL)
      goto error;

    /* Recieve and store the argument */
    if (znsock_recv(socket, message->argv[i], size) == -1)
      goto error;

    i++;
  }

  return false;

  error:
    /* Cleanup all variables allocated in message */
    if (message->argc > 0) {
      unsigned int i = 0;
      while (i < message->argc) {
        if (message->argv[i])
          free(message->argv[i]);

        i++;
      }
    }

    if (message->argv != NULL)
      free(message->argv);

    return true;
}

bool message_send(znsock socket, socket_message message)
{
  if (znsock_send(socket, &message.argc, sizeof(unsigned int)) == -1)
    return true;

  int i = 0;
  while (i < message.argc) {
    char *str = message.argv[i];
    unsigned int l = strlen(str) + 1;

    if (znsock_send(socket, &l, sizeof(unsigned int)) == -1)
      return true;

    if (znsock_send(socket, str, l) == -1)
      return true;

    i++;
  }

  return false;
}

void message_free(socket_message message)
{
  unsigned int c = message.argc;

  int i = 0;
  while (c > i) {
    free(message.argv[i]);
    i++;
  }

  free(message.argv);
}
