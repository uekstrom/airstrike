# Make an archive of all binary files (they are not in CVS).

FILES:=$(shell find . -name '*.jpg' -o -name '*.png' -o -name '*.wav')

airstrike-media.tar.gz: $(FILES)
	tar czvf $@ $^
