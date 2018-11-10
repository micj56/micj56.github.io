#
# Makefile for SPL project
#

VERSION = 1.0

DIRS = Dokumente Aufgaben Loesungen

all:
		for i in $(DIRS) ; do $(MAKE) -C $$i ; done

clean:
		for i in $(DIRS) ; do $(MAKE) -C $$i clean ; done
		rm -f *~

dist-clean:
		for i in $(DIRS) ; do $(MAKE) -C $$i dist-clean ; done
		rm -f *~

dist:		dist-clean
		(cd .. ; \
		 tar -cvf spl-$(VERSION).tar spl-$(VERSION)/* ; \
		 gzip -f spl-$(VERSION).tar)
