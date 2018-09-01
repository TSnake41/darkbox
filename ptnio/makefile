# ptnio - Portable TCP Network IO
# Copyright (C) 2017-2018 Teddy ASTIE
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

CFLAGS ?= -O2
LDFLAGS ?= -O2 -s

LDFLAGS += -I$(libdir) -I. -lpthread

libdir=../lib

EXEC=ptnio$(exec_suffix)
BINDIR=../bin

lib_src=$(libdir)/sthread.c $(libdir)/core_i.c $(libdir)/znsock.c \
	$(libdir)/nms.c $(libdir)/tiny_assert.c

src=ptnio.c args_parser.c message.c pipe_fork.c \
	client/*.c server/*.c server/commands/*.c

all: ptnio postbuild

ptnio:
	$(CC) -o $(EXEC) $(CFLAGS) $(lib_src) $(src) $(LDFLAGS)

postbuild:
	test -d $(BINDIR) || mkdir $(BINDIR)
	mv $(EXEC) ../bin
	cp $(BINDIR)/$(EXEC) ../tests/

.PHONY: $(EXEC)
