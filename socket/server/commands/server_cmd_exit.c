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

#include <stdlib.h>
#include <socket_ipc.h>

#include "server_cmd_utils.h"

/* Syntax : exit
   Usage : Ends server.
*/
void server_cmd_exit(socket_message msg, socket_int client, server_data *data)
{
  /* Lock mutex (lock other thread). */
  smutex_unlock(&data->pair_mutex);

  /* Close IPC socket, thus disabling IPC. */
  close(client);
  close(data->ipc_socket);

  /* Force all sockets to close. */
  size_t i = data->pair_count;
  while (i--) {
    close(data->pair_list[0]->socket);
    server_remove_pair_unlocked(data, 0);
  }

  /* Exit current process */
  exit(0);
}
