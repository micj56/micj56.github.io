#
# Makefile for assembling the master boot record
#

SFILES = $(wildcard *.s)
BINFILES = $(patsubst %.s,%.bin,$(SFILES))

all:	$(BINFILES)

%.bin:	%.s
	../../../build/bin/asld -h -rc 0xC0000400 -o $@ $<

clean:
	rm -f *~ *.bin
