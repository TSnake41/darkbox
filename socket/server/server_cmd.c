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

#include "server_cmd.h"

void server_cmd_new(socket_message, socket_int, server_data *);
void server_cmd_free(socket_message, socket_int, server_data *);
void server_cmd_accept(socket_message, socket_int, server_data *);

void server_cmd_recv(socket_message, socket_int, server_data *);
void server_cmd_send(socket_message, socket_int, server_data *);

void server_cmd_nms_recv(socket_message, socket_int, server_data *);
void server_cmd_nms_send(socket_message, socket_int, server_data *);

void server_cmd_bind(socket_message, socket_int, server_data *);
void server_cmd_listen(socket_message, socket_int, server_data *);

void server_cmd_connect(socket_message, socket_int, server_data *);

void server_cmd_list(socket_message, socket_int, server_data *);
void server_cmd_info(socket_message, socket_int, server_data *);

void server_cmd_poll(socket_message, socket_int, server_data *);

void server_cmd_exit(socket_message, socket_int, server_data *);

const server_cmd server_cmds[] = {
  { server_cmd_new, "new" },
  { server_cmd_free, "free" },
  { server_cmd_free, "close" }, /* free alias */
  { server_cmd_accept, "accept" },
  { server_cmd_bind, "bind" },
  { server_cmd_listen, "listen" },
  { server_cmd_recv, "recv" },
  { server_cmd_send, "send" },
  { server_cmd_connect, "connect" },
  { server_cmd_list, "list" },
  { server_cmd_info, "info" },
  { server_cmd_poll, "poll" },
  { server_cmd_nms_recv, "nms_recv" },
  { server_cmd_nms_send, "nms_send" },
  { server_cmd_exit, "exit" },
};
const unsigned int server_cmds_count = sizeof(server_cmds) / sizeof(*server_cmds);
