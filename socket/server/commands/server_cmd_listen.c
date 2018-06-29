/*
    SockeT - Portable TCP and NMS Network IO interface.
    Copyright (c) 2017-2018 Teddy ASTIE (TSnake41)

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

#include "server_cmd_utils.h"

/* Syntax : listen sock_id [n]
   Usage : Prepare to accept connections, allow n pending requests.
*/
void server_cmd_listen(socket_message msg, znsock client, server_data *data)
{
  if (msg.argc < 2) {
    /* Invalid arguments */
    send_code(client, CMD_INVALID_ARGS);
    return;
  }

  id_socket_pair *listner_pair = server_get_pair(data, msg.argv[1], NULL);

  if (listner_pair == NULL) {
    /* No pair */
    send_code(client, CMD_NOT_FOUND);
    return;
  }

  /* Define n (if not defined in args, define it to 0) */
  int n = msg.argc > 2 ? strtol(msg.argv[2], NULL, 0) : 0;

  if (listen(listner_pair->socket, n) == -1) {
    send_code(client, CMD_NETWORK_ERROR);
    return;
  }

  send_code(client, CMD_SUCCESS);
}
