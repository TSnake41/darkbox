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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <windows.h>
#include <socket.h>
#include <socket_ipc.h>

/*
    Create a file named $socket_[id] that store the port.
    This file will be read by the socket_client to establish
    the connection with the server.
*/

static const char *file_id_prefix = "$socket_";

static void get_file_id_path(const char *id, char *out)
{
  strcpy(out, file_id_prefix);
  strcat(out, id);
}

socket_int socket_ipc_server_new(const char *id, int max_pending)
{
  char f_path[strlen(file_id_prefix) + strlen(id) + 1];
  get_file_id_path(id, f_path);

  socket_int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == INVALID_SOCKET)
    return INVALID_SOCKET;

  struct sockaddr_in sin = { 0 };
  sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  sin.sin_family = AF_INET;
  sin.sin_port = 0x0000;

  if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) == SOCKET_ERROR) {
    closesocket(s);
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

socket_int socket_ipc_client_new(const char *id)
{
  char f_path[strlen(file_id_prefix) + strlen(id) + 1];
  get_file_id_path(id, f_path);

  unsigned short port;

  FILE *f = fopen(f_path, "r");
  if (f == NULL)
    return INVALID_SOCKET;

  fread(&port, sizeof(unsigned short), 1, f);
  fclose(f);

  socket_int s;
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

  /* Set non-blocking */
  u_long non_blocking = 0;
  ioctlsocket((int)socket, FIONBIO, &non_blocking);
  return s;
}

socket_int socket_ipc_server_accept(socket_int server)
{
  socket_int s = accept(server, NULL, NULL);
  if (socket_is_valid(s)) {
    u_long non_blocking = 0;
    ioctlsocket((int)s, FIONBIO, &non_blocking);
  }

  return s;
}
