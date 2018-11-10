#
# Makefile to make a new disk with a master boot record on it
#

DSKNAME = system.disk
DSKSIZE = 14000
MBRNAME = mbr.bin

all:		$(DSKNAME)

$(DSKNAME):	mkdisk/mkdisk wrtmbr/wrtmbr mkmbr/$(MBRNAME)
		mkdisk/mkdisk $(DSKNAME) $(DSKSIZE)
		wrtmbr/wrtmbr $(DSKNAME) mkmbr/$(MBRNAME)

mkdisk/mkdisk:
		$(MAKE) -C mkdisk

wrtmbr/wrtmbr:
		$(MAKE) -C wrtmbr

mkmbr/$(MBRNAME):
		$(MAKE) -C mkmbr

clean:
		$(MAKE) -C mkdisk clean
		$(MAKE) -C wrtmbr clean
		$(MAKE) -C mkmbr clean
		rm -f *~ $(DSKNAME)
