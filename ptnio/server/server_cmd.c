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

#include "server_cmd.h"

void server_cmd_new(socket_message, znsock, server_data *);
void server_cmd_free(socket_message, znsock, server_data *);
void server_cmd_accept(socket_message, znsock, server_data *);

void server_cmd_recv(socket_message, znsock, server_data *);
void server_cmd_send(socket_message, znsock, server_data *);

void server_cmd_nms_recv(socket_message, znsock, server_data *);
void server_cmd_nms_send(socket_message, znsock, server_data *);

void server_cmd_bind(socket_message, znsock, server_data *);
void server_cmd_listen(socket_message, znsock, server_data *);

void server_cmd_connect(socket_message, znsock, server_data *);

void server_cmd_list(socket_message, znsock, server_data *);
void server_cmd_info(socket_message, znsock, server_data *);

void server_cmd_poll(socket_message, znsock, server_data *);

void server_cmd_exit(socket_message, znsock, server_data *);

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
