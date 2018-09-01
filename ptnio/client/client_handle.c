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
#include <stdint.h>
#include <nms.h>
#include <znsock.h>

#ifndef WIN32
#include <core_i.h>
#endif

#ifdef WIN32
#include <windows.h>
int _setmode(int, int);
#define set_bin_mode(f) _setmode(_fileno(f), 0x8000)
#endif

#include "client_utils.h"
#include "client_handle.h"
#include "../cmd_codes.h"

#define client_handle_nms_send client_handle_send

const client_handle client_handles[] = {
  { client_handle_code, "new" },
  { client_handle_code, "free" },
  { client_handle_nms_recv, "accept" },
  { client_handle_code, "bind" },
  { client_handle_code, "listen" },
  { client_handle_recv, "recv" },
  { client_handle_send, "send" },
  { client_handle_code, "connect" },
  { client_handle_recv, "list" },
  { client_handle_nms_recv, "info" },
  { client_handle_recv, "poll" },
  { client_handle_nms_recv, "nms_recv" },
  { client_handle_nms_send, "nms_send" },
};
const unsigned int client_handles_count = sizeof(client_handles) / sizeof(*client_handles);

/** Recieve code from socket, and use it as exit code. */
void client_handle_code(znsock socket)
{
  uint8_t code = recv_code(socket);
  znsock_ipc_close(socket);
  exit(code);
}

/** First recieve code from IPC socket, then (if sucess)
 *  read message write them to stdout until count!=0xFFFF
 */
void client_handle_recv(znsock socket)
{
  uint8_t code = recv_code(socket);
  if (code != CMD_SUCCESS) {
    /* Is an error. */
    znsock_ipc_close(socket);
    exit(code);
  }

  #ifdef WIN32
  /* Set stdout to binary mode */
  set_bin_mode(stdout);
  #endif

  uint16_t count = 0;
  char *buffer = malloc(0xFFFF);
  if (buffer == NULL) {
    znsock_ipc_close(socket);
    exit(CMD_CLIENT_OOM);
  }

  do {
    if (nms_recv_no_alloc(socket, buffer, &count))
      exit(CMD_IPC_ERROR);

    fwrite(buffer, count, 1, stdout);
  } while(count != 0);

  free(buffer);
  znsock_ipc_close(socket);
  exit(CMD_SUCCESS);
}

/** Send to IPC socket data from stdin. */
void client_handle_send(znsock socket)
{
  setvbuf(stdin, NULL, _IONBF, 0);

  #ifdef WIN32
	/* Set stdin to binary mode */
  set_bin_mode(stdin);
  #endif

  int count = 0;
  char *buffer = malloc(0xFFFF);
  if (buffer == NULL) {
    znsock_ipc_close(socket);
    exit(CMD_CLIENT_OOM);
  }

  uint8_t code = recv_code(socket);
  if (code == CMD_SUCCESS) {
    do {
      count = fread(buffer, 1, 0xFFFF, stdin);

      if (nms_send(socket, buffer, count))
        exit(CMD_IPC_ERROR);

      code = recv_code(socket);
    } while(count != 0 && code == CMD_SUCCESS);
  }

  free(buffer);

  znsock_ipc_close(socket);
  exit(code);
}

/** Recieve code from IPC socket, then (if success)
 *  recieve network message and write it to stdout.
 */
void client_handle_nms_recv(znsock socket)
{
  uint8_t code = recv_code(socket);
  if (code != CMD_SUCCESS)
    /* Is an error. */
    exit(code);

  #ifdef WIN32
  /* Set stdout to binary mode */
  set_bin_mode(stdout);
  #endif

  uint16_t recieved = 0;
  void *buffer;

  if (nms_recv(socket, &buffer, &recieved))
    exit(CMD_IPC_ERROR);
  else
    fwrite(buffer, 1, recieved, stdout);

  free(buffer);
  znsock_ipc_close(socket);
  exit(recieved ? CMD_SUCCESS : CMD_NMS_ZERO_SIZE);
}
