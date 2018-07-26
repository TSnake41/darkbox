/*
    ptnio - Portable TCP Network IO
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
