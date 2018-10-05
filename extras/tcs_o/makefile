# Copyright (C) 2018 Teddy ASTIE
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

CFLAGS ?= -O2 -s
LDFLAGS ?= -s

LDFLAGS += -I$(libdir)

libdir=../../lib

BINDIR=../bin

lib_src=$(libdir)/core.c
src=ctcs_o_core.c

EXEC=ctcs_o_core$(exec_suffix)

all: ctcs_o_core postbuild

ctcs_o_core:
	$(CC) -o $(EXEC) $(CFLAGS) $(lib_src) $(src) $(LDFLAGS)

postbuild:
	test -d $(BINDIR) || mkdir $(BINDIR)
	mv ctcs_o_core$(exec_suffix) ../bin

.PHONY: $(EXEC)
