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

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#include <io.h>
#include <process.h>
#include <fcntl.h>
#endif

int pipe_fork(int argc, char **argv)
{
  #ifdef NO_FORK
  return 0;
  #else
  #ifndef WIN32
  int pipes[2];
  pipe(pipes);

  if (fork() != 0) {
    /* Wait until child writes something in the pipe. */
    char b;
    read(pipes[0], &b, 1);

    /* Close write pipes. */
    close(pipes[0]);
    close(pipes[1]);

    setsid();
    exit(0);
  }

  /* Close read pipe */
  close(pipes[0]);
  return pipes[1];
  #else
  if (strncmp(argv[1], ".pf:", 4) != 0) {
    /* Create pipes */
    int pipes[2];
    _pipe(pipes, 32, O_BINARY);

    char buffer[16];
    /* Write to buffer write pipe. */
    snprintf(buffer, sizeof(buffer), ".pf:%x", pipes[1]);

    char *(new_argv[argc + 1]);
    new_argv[0] = argv[0];
    new_argv[1] = buffer;
    memcpy(&new_argv[2], &argv[1], argc * sizeof(char *));

    if (spawnvp(P_NOWAIT, new_argv[0], new_argv) == -1) {
      fputs("ERROR: Unable to spawn new process.", stderr);
      exit(1);
    }

    /* Wait response */
    read(pipes[0], buffer, 1);

    /* Close pipes */
    close(pipes[0]);
    close(pipes[1]);

    exit(0);
  }

  /* Return pipe fd read from argv[1] */
  const char *str = strchr(argv[1], ':') + 1;
  return strtol(str, NULL, 0);
  #endif
  #endif
}

void pipe_send(int fd)
{
  /* Notify parent */
  char b = 1;
  write(fd, &b, 1);
  close(fd);
}
