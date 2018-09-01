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

/*
    Interprocess communication for SockeT.

    Implementation differ between POSIX and Windows :
     - POSIX implementation uses Unix Domain sockets
     - Windows implementation uses TCP/IP.
*/

#if !defined(IPC_USE_UNIX) && !defined(IPC_USE_TCP)
  #ifndef WIN32
  #define IPC_USE_UNIX
  #else
  #define IPC_USE_TCP
  #endif
#endif

#ifdef IPC_USE_UNIX
#include "ipc/ipc_unix.c"
#elif defined(IPC_USE_TCP)
#include "ipc/ipc_tcp.c"
#else
#error Invalid IPC backend.
#endif
