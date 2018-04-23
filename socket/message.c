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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <socket_ipc.h>

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

#define msg_flags socket_default_flags

bool message_recv(socket_int socket, socket_message *message)
{
  if (message == NULL)
    goto error;

  /* Recieve the argument count. */
  if (recv(socket, &message->argc, sizeof(unsigned int), msg_flags) == -1)
    goto error;

  message->argv = calloc(message->argc, sizeof(char *));
  if (message->argv == NULL)
    goto error;

  unsigned int i = 0;
  while (i < message->argc) {
    /* Recieve the argument size (including NULL terminator). */
    unsigned int size;
    if (recv(socket, &size, sizeof(unsigned int), msg_flags) == -1)
      goto error;

    /* Allocate memory for argument */
    message->argv[i] = malloc(size);
    if (message->argv[i] == NULL)
      goto error;

    /* Recieve and store the argument */
    if (recv(socket, message->argv[i], size, msg_flags) == -1)
      goto error;

    i++;
  }

  return false;

  error:
    /* Cleanup all variables allocated in message */
    if (message->argc) {
      unsigned int i = 0;
      while (i < message->argc) {
        if (message->argv[i])
          free(message->argv);

        i++;
      }
    }

    if (message->argv)
      free(message->argv);

    return true;
}

bool message_send(socket_int socket, socket_message message)
{
  if (send(socket, &message.argc, sizeof(unsigned int), msg_flags) == -1)
    return true;

  int i = 0;
  while (i < message.argc) {
    char *str = message.argv[i];
    unsigned int l = strlen(str) + 1;

    if (send(socket, &l, sizeof(unsigned int), msg_flags) == -1)
      return true;

    if (send(socket, str, l, msg_flags) == -1)
      return true;

    i++;
  }

  return false;
}

void message_free(socket_message message)
{
  int c = message.argc;

  int i = 0;
  while (c > i) {
    free(message.argv[i]);
    i++;
  }

  free(message.argv);
}
