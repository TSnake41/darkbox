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

#ifndef H_CLIENT_HANDLE
#define H_CLIENT_HANDLE

#include <znsock.h>

typedef void (*client_handle_func)(znsock);

typedef struct client_handle {
  client_handle_func func;
  char *cmd;
} client_handle;

void client_handle_code(znsock socket);

void client_handle_recv(znsock socket);
void client_handle_send(znsock socket);

void client_handle_nms_recv(znsock socket);
void client_handle_nms_send(znsock socket);

extern const unsigned int client_handles_count;
extern const client_handle client_handles[];

#endif /* H_CLIENT_HANDLE */
