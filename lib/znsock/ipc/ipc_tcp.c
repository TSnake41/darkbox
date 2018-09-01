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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <windows.h>
#include <znsock.h>

/*
    Create a file named $socket_[id] that store the port.
    This file will be read by the socket_client to establish
    the connection with the server.
*/

static const char *file_id_prefix = "$ptnio_";

static void get_file_id_path(const char *id, char *out)
{
  strcpy(out, file_id_prefix);
  strcat(out, id);
}

znsock znsock_ipc_server(const char *id, int max_pending)
{
  char f_path[strlen(file_id_prefix) + strlen(id) + 1];
  get_file_id_path(id, f_path);

  znsock s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == INVALID_SOCKET)
    return INVALID_SOCKET;

  struct sockaddr_in sin = { 0 };
  sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  sin.sin_family = AF_INET;
  sin.sin_port = 0x0000;

  if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
    znsock_close(s, false);
    return INVALID_SOCKET;
  }

  FILE *f = fopen(f_path, "w");
  if (f == NULL) {
    closesocket(s);
    return INVALID_SOCKET;
  }

  listen(s, max_pending);

  int len = sizeof(sin);
  getsockname(s, (struct sockaddr *)&sin, &len);

  fwrite(&sin.sin_port, sizeof(unsigned short), 1, f);
  fclose(f);

  return s;
}

znsock znsock_ipc_client(const char *id)
{
  char f_path[strlen(file_id_prefix) + strlen(id) + 1];
  get_file_id_path(id, f_path);

  unsigned short port;

  FILE *f = fopen(f_path, "r");
  if (f == NULL)
    return INVALID_SOCKET;

  fread(&port, sizeof(unsigned short), 1, f);
  fclose(f);

  znsock s;
  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    return INVALID_SOCKET;

  struct sockaddr_in sin = { 0 };
  sin.sin_family = AF_INET;
  sin.sin_addr.S_un.S_addr = htonl(INADDR_LOOPBACK);
  sin.sin_port = port;

  if (connect(s, (struct sockaddr *)&sin,
    sizeof(struct sockaddr)) == SOCKET_ERROR) {

    closesocket(s);
    return INVALID_SOCKET;
  }

  return s;
}

znsock znsock_ipc_accept(znsock s)
{
  return accept(s, NULL, NULL);
}

void znsock_ipc_close(znsock s)
{
  znsock_close(s, true);
}
