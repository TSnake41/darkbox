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

#include <znsock.h>

static znsock znsock_ipc_socket(const char *id,
  struct sockaddr_un *saun_o)
{
  const char *f_path_prefix = ".ptnio_";
  strcpy(saun_o->sun_path, f_path_prefix);
  strcat(saun_o->sun_path, id);

  saun_o->sun_family = AF_UNIX;

  if ((strlen(id) + strlen(f_path_prefix) + 1) > 108) {
    fputs("IPC: ID too long !\n", stderr);
    return -1;
  }

  znsock s;
  if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    return -1;

  return s;
}

znsock znsock_ipc_server(const char *id, int max_pending)
{
  znsock s;
  struct sockaddr_un saun;
  if ((s = znsock_ipc_socket(id, &saun)) == 0)
    return -1;

  socklen_t l = sizeof(saun.sun_family) + strlen(saun.sun_path);

  unlink(saun.sun_path);

  if (bind(s, (const struct sockaddr *)&saun, l) < 0) {
    close(s);
    return -1;
  }

  listen(s, max_pending);
  return s;
}

znsock znsock_ipc_client(const char *id)
{
  znsock s;
  struct sockaddr_un saun;

  if ((s = znsock_ipc_socket(id, &saun)) == -1)
    return -1;

  if (connect(s, (struct sockaddr *)&saun, sizeof(struct sockaddr_un)) == -1){
    close(s);
    return -1;
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
