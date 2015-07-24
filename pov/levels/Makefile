#-------------------------------------------------------------
# default image sizes if they're not given on the command line
ifndef WD
WD := 800
endif
ifndef HT
HT := 600
endif

# antialias, no dithering, global includes
POVRAY = povray -J -L../include +W$(WD) +H$(HT)
POVRAYMASK = $(POVRAY) +Q0
POVRAYP = $(POVRAY) +A

CONVERT = convert
COMPOSITE = composite

# ------------------- generic rules --------------------------

# all models
POV = $(wildcard *.pov)
PNG = $(patsubst %.pov, %.png, $(POV))
MSK = $(patsubst %.pov, %-mask.png, $(POV))

all: $(PNG) $(MSK)

# default rule for background image masks:
# - lowest quality, use mask include (for mask defines)
%-mask.png: %.pov
	cp $< mask.pov
	$(POVRAYMASK) +HImask.inc +Imask.pov
	$(CONVERT) mask.png -threshold 1 $@
	$(RM) mask.pov mask.png

# default rule for background images:
# - full quality with antialias and mosaic preview
%.png: %.pov
	$(POVRAYP) +SP4 +EP4 +I$<

# --------------- special rules ------------------------------

hills2-fg.png: hills2.pov
	$(POVRAYP) +ua +i$< +o$@ declare=NO_MOUNTAINS=1 declare=NO_SKY=1 declare=NO_FOG=1
hills2-bg.png: hills2.pov
	$(POVRAYP) +ua -a +i$< +o$@ declare=NO_HILLS=1

# depends on Makefile so that I can easily regenerate these
hills2.png: hills2-fg.png hills2-bg.png Makefile
	# blur mountains & sky
	$(CONVERT) -blur 1.2,1.2 hills2-bg.png tmp.png
	# add grass
	$(COMPOSITE) -compose over hills2-fg.png tmp.png $@
	$(RM) tmp.png

# --------------- extra dependencies -------------------------

hills.png: grass.inc
island.png: grass.inc
doughnut.png: grass.inc

# ------------------------------------------------------------

# the order and names of the (renamed) levels
PICS = hills platforms island the-pit doughnut hills2
# where to install levels
DIR = ../../share/levels
# thumbnail size
THUMB = 80x60

# for more info on install, see README
install: all
	./install-levels.sh $(DIR) $(THUMB) $(PICS)

clean:
	$(RM) *~ *.bak

distclean: clean
	$(RM) *.png
