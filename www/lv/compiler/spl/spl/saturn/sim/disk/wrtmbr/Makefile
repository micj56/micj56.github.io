#
# Makefile for a program which writes the master boot record to the disk
#

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -g
LDLIBS = -lm

SRCS = wrtmbr.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = wrtmbr

.PHONY:		all clean

all:		$(BIN)

$(BIN):		$(OBJS)
		$(LINK.o) $^ $(LDLIBS) -o $@

depend.mak:
		$(CC) -MM -MG $(CFLAGS) $(SRCS) > depend.mak

-include depend.mak

clean:
		rm -f *~ $(OBJS) $(BIN) depend.mak
