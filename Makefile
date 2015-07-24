#some variables for the install target
ETC:=/usr/local/etc
PREFIX:=/usr/local
DATADIR:=share/airstrike
BINDIR:=bin

# always regenerate
.PHONY: airstrike airstrike-nosound

airstrike:
	$(MAKE) -C src USE_SOUND=1 OBJ_DEBUG=0 EFENCE=0 READLINE=0 airstrike
	mv src/airstrike airstrike

# note: do 'make clean' between sound changing
airstrike-nosound:
	$(MAKE) -C src USE_SOUND=0 OBJ_DEBUG=0 EFENCE=0 READLINE=0 airstrike
	mv src/airstrike airstrike-nosound

clean:
	$(RM) $(shell find . -name '*.[od]' -o -name '*~' -o -name '#*' -o -name 'dep') airstrike airstrike-nosound

distclean: clean
	$(RM) -rf $(shell find . -name .xvpics)
	$(RM) -f *.bmp *.png
	$(MAKE) -C pov distclean

# package name and what to exclude from it
# TODO: these targets need a total overhaul
PKG_NAME = airstrike-$(shell date -I)
EXCLUDE = --exclude CVS --exclude '*/pov/*.png' --exclude '*/pov/*/*.png' --exclude '*/contrib*' --exclude '*/share/*/sprites-*' --exclude '*.old*'

package: clean
	cd ..; \
	mv airstrike $(PKG_NAME); \
	tar -zcvf $(PKG_NAME).tar.gz $(EXCLUDE) $(PKG_NAME); \
	mv $(PKG_NAME) airstrike

# packages new files listed in new-files.txt
new:
	tar -zcvf new.tgz -T new-files.txt

install: airstrike
	echo "Sorry, no installation target yet."
	echo "Run the game from this directory."
#	install airstrike $(PREFIX)/$(BINDIR)
#	install airstrikerc $(ETC)
#	cp -r share $(PREFIX)/$(DATADIR)
#	echo \"datapath $(PREFIX)/$(DATADIR)\" >> $(ETC)/airstrikerc

# an install target should be accompanied by an uninstall target
# uninstall:
