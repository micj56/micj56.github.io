#
# Makefile for SPL compiler
#

CC = gcc
CFLAGS = -Wall -Wno-unused -g
LDFLAGS = -g
LDLIBS = -lm

SRCS = main.c utils.c lex.yy.c
OBJS = $(patsubst %.c,%.o,$(SRCS))
BIN = spl

.PHONY:		all tests depend clean dist-clean

all:		$(BIN)

$(BIN):		$(OBJS)
		$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<

lex.yy.c:	scanner.l
		flex scanner.l

tests:		all
		@for i in Tests/test??.spl ; do \
		  echo ; \
		  ./$(BIN) $$i ; \
		done
		@echo

-include depend.mak

depend:		lex.yy.c
		$(CC) $(CFLAGS) -MM $(SRCS) > depend.mak

clean:
		rm -f *~ *.o
		rm -f Tests/*~

dist-clean:	clean
		rm -f $(BIN) lex.yy.c depend.mak
