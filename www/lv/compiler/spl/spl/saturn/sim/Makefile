#
# Makefile for ECO32 simulator
#

CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -L/usr/X11/lib -g
LDLIBS = -lcurses -lX11 -lpthread -lrt -lm

SRCS = main.c curslib.c error.c except.c command.c \
       instr.c asm.c disasm.c cpu.c mmu.c memory.c \
       timer.c term.c disk.c graph.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = sim

.PHONY:		all clean

all:		$(BIN)
		#cp $(BIN) ../build/bin
		#cp disk/mkdisk/mkdisk ../build/bin
		#cp disk/wrtmbr/wrtmbr ../build/bin
		#cp disk/mkmbr/mbr.bin ../build/run
		#cp disk/system.disk ../build/run
		#cp rom/bootmon.bin ../build/run

disk/system.disk:
		$(MAKE) -C disk

rom/bootmon.bin:
		$(MAKE) -C rom

$(BIN):		$(OBJS)
		$(LINK.o) $^ $(LDLIBS) -o $@

depend.mak:
		$(CC) -MM -MG $(CFLAGS) $(SRCS) > depend.mak

-include depend.mak

clean:
		#$(MAKE) -C disk clean
		#$(MAKE) -C rom clean
		rm -f *~ $(OBJS) $(BIN) depend.mak
