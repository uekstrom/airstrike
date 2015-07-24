
# default sizes if they're not given on the command line
ifndef WD
WD := 48
endif
ifndef HT
HT := 48
endif

POVRAYP = povray +UA +A -J -L../include

# ------------------------------------------------------------

# all models
POV = $(wildcard *.pov)
PNG = $(patsubst %.pov, %.png, $(POV))

all: $(PNG)

# default rule for other POV files
%.png: %.pov
	$(POVRAYP) +I$< +W$(WD) +H$(HT)

# ------------------------------------------------------------

as-textures.png: as-textures.pov ../include/as-textures.inc
	$(POVRAYP) +I$< +W400 +H400

brick.png: brick.pov
	$(POVRAYP) +I$< +W96 +H96

cactus.png: cactus.pov
	$(POVRAYP) +I$< +W32 +H64

flatground.png: flatground.pov
	$(POVRAYP) +I$< +W64 +H64

glacier.png: glacier.pov
	$(POVRAYP) +I$< +W256 +H256

grassyground.png: grassyground.pov
	$(POVRAYP) +I$< +W64 +H64

hill.png: hill.pov
	$(POVRAYP) +I$< +W640 +H480

icefloat.png: icefloat.pov
	$(POVRAYP) +I$< +W256 +H64

island.png: island.pov
	$(POVRAYP) +I$< +W256 +H128

igloo.png: igloo.pov
	$(POVRAYP) +I$< +W64 +H32

palmtree.png: palmtree.pov
	$(POVRAYP) +I$< +W256 +H256 

redwood.png: redwood.pov
	$(POVRAY) +I$< +W32 +H96

schrub.png: schrub.pov
	$(POVRAYP) +I$< +W32 +H28

tower.png: tower.pov
	$(POVRAYP) +I$< +W64 +H128

waves.png: waves.pov
	$(POVRAYP) +I$< +W200 +H200

# ------------------------------------------------------------

install: all
	$(CP) *.png ../../share/graphics/background/

clean:
	$(RM) *~ *.bak

distclean: clean
	$(RM) *.png
