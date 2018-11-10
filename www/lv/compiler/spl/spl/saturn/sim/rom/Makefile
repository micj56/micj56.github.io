#
# Makefile for assembling different system ROMs
#

SFILES = $(wildcard *.s)
BINFILES = $(patsubst %.s,%.bin,$(SFILES))

all:	$(BINFILES)

%.bin:	%.s
	../../build/bin/asld -h -rc 0xFE000000 -o $@ $<

clean:
	rm -f *~ *.bin
