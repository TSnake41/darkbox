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

CFLAGS ?= -O2 -s
LDFLAGS ?= -s

LDFLAGS += -I$(libdir)

libdir=../../lib
BINDIR=../bin

EXEC_FR=choice_fr$(exec_suffix)
EXEC_EN=choice_en$(exec_suffix)
EXEC_DE=choice_de$(exec_suffix)

EXEC_ALL=$(EXEC_FR) $(EXEC_EN) $(EXEC_DE)

lib_src=$(libdir)/core.c $(libdir)/core_i.c
src=choice.c parse_args.c strcasechr.c

all: choice_en choice_fr choice_de postbuild

choice_en:
	$(CC) -o $(EXEC_EN) $(CFLAGS) $(lib_src) $(src) $(LDFLAGS)

choice_fr:
	$(CC) -o $(EXEC_FR) $(CFLAGS) $(lib_src) $(src) $(LDFLAGS) -DLANG_FR

choice_de:
	$(CC) -o $(EXEC_DE) $(CFLAGS) $(lib_src) $(src) $(LDFLAGS) -DLANG_DE

postbuild:
	test -d $(BINDIR) || mkdir $(BINDIR)
	cp $(EXEC_ALL) ../tests
	mv $(EXEC_ALL) ../bin

.PHONY: $(EXEC)
