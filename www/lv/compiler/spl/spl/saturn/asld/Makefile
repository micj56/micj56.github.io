#
# Makefile for ECO32 assembler
#

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -g
LDLIBS = -lm

SRCS = asld.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = asld

.PHONY:		all clean

all:		$(BIN)
		#cp $(BIN) ../build/bin

$(BIN):		$(OBJS)
		$(LINK.o) $^ $(LDLIBS) -o $@

depend.mak:
		$(CC) -MM -MG $(CFLAGS) $(SRCS) > depend.mak

-include depend.mak

clean:
		rm -f *~ $(OBJS) $(BIN) depend.mak
