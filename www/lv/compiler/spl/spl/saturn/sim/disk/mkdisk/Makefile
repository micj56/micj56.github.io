#
# Makefile for disk creator
#

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -g
LDLIBS = -lm

SRCS = mkdisk.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = mkdisk

.PHONY:		all clean

all:		$(BIN)

$(BIN):		$(OBJS)
		$(LINK.o) $^ $(LDLIBS) -o $@

depend.mak:
		$(CC) -MM -MG $(CFLAGS) $(SRCS) > depend.mak

-include depend.mak

clean:
		rm -f *~ $(OBJS) $(BIN) depend.mak
