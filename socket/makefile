# SockeT - Portable TCP and NMS Network IO interface.
# Copyright (c) 2017 Teddy ASTIE (TSnake41)
#
# All rights reserved.
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
# * Neither the name of the name of Teddy Astie (TSnake41) nor the
#   names of its contributors may be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY TEDDY ASTIE AND CONTRIBUTORS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL TEDDY ASTIE AND CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES
# LOSS OF USE, DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CFLAGS=-O2 -s
libdir=../lib
LDFLAGS=-I$(libdir) -I.

ifeq ($(OS),Windows_NT)
	LDFLAGS += -lws2_32
else
	LDFLAGS += -pthread
endif

EXEC=socket$(exec_suffix)
BINDIR=../bin

lib_src=$(libdir)/sthread/sthread.c $(libdir)/fllist/fllist.c \
	    $(libdir)/socket/socket.c $(libdir)/nms/nms.c \
	 	$(libdir)/ipc/ipc.c	$(libdir)/core_i/core_i.c

src=socket.c args_parser.c message.c \
	client/*.c server/*.c \
	server/commands/*.c

all: socket postbuild

socket:
	$(CC) -o $(EXEC) $(CFLAGS) $(lib_src) $(src) $(LDFLAGS)

postbuild:
	test -d $(BINDIR) || mkdir $(BINDIR)
	mv $(EXEC) ../bin
	cp $(BINDIR)/$(EXEC) ../tests/network/

.PHONY: $(EXEC)
