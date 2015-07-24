# Makefile for creating Airstrike game graphics data
# from the PovRay v3.5 3D models. Requires _GNU_ make.
#
# Before you add animation creation in this Makefile, you
# can test animating your 'name.pov' with a command like:
#   make NAME=name WD=xx HT=yy FRAMES=kk OPT='options' BLUR=1 anim
#
# WD, HT, OPT and BLUR parameters are optional. If you don't want
# povray to wait for user click after each rendering, give
# BATCH=1 argument.
#
# (w) 2002 by Ulf Ekström & Eero Tamminen

# ---------------- default values -------------------------

ifndef WD
WD := 48
endif

ifndef HT
HT := 48
endif

# PovRay with default flags
POVRAYFAST = povray +FN +DTC -J +UA -P -Linclude
POVRAY = $(POVRAYFAST) +A0.3

ifdef BATCH
POVRAYP = $(POVRAY)
else
# wait until user clicks
POVRAYP = $(POVRAY) +P
endif

# ------------------------------------------------------------

ifdef NAME

# I'm using submake to create animations because with
# shell-script you'd need to separately handle errors
# and Make does it automatically.

anim:
	$(RM) $(NAME).png
	$(POVRAY) +I$(NAME).pov +W$(WD) +H$(HT) +KFF$(FRAMES) $(OPT)
ifdef BLUR
	../utils/blur.sh $(NAME)[0-9]*.png
endif
	convert +append $(NAME)[0-9]*.png $(NAME).png
	$(RM) $(NAME)[0-9]*.png

else

# ------------------------------------------------------------

# images created in special ways
SPECIAL = bonus.png whitesmoke.png \
	redplane.png redplane-lr.png # redplanewreck.png

# all models
POV = $(wildcard *.pov)
PNG = $(patsubst %.pov, %.png, $(POV)) $(SPECIAL)

.PHONY: levels
all: $(PNG) levels

# default rule for other POV files
%.png: %.pov
	$(POVRAYP) +I$< +W$(WD) +H$(HT)

# -------------------- animations -----------------------------

# explicit rules are separated into animations that use submake
# and single frames which just use povray.  They are sorted
# alphabetically so that the rules are easier to find and in
# the same order as Make will execute them with the 'all' rule.

airballoon.png: airballoon.pov
	$(MAKE) NAME=airballoon WD=64 HT=64 FRAMES=8 OPT=+KC anim

asteroid.png: asteroid.pov
	$(MAKE) NAME=asteroid WD=32 HT=32 FRAMES=24 OPT=+KC anim

asteroid-large.png: asteroid-large.pov
	$(MAKE) NAME=asteroid-large WD=64 HT=64 FRAMES=64 anim OPT=+KC

balloon-deflate.png: balloon-deflate.pov
	$(MAKE) NAME=balloon-deflate WD=32 HT=32 FRAMES=32 OPT=+KC anim

balloon-bounce.png: balloon-bounce.pov
	$(MAKE) NAME=balloon-bounce WD=32 HT=32 FRAMES=32 OPT=+KC anim

bird.png: bird.pov
	$(MAKE) NAME=bird WD=12 HT=12 FRAMES=8 anim

redplane.png blueplane.png: blueplane.pov plane.inc
	$(MAKE) NAME=blueplane WD=48 HT=48 FRAMES=64 OPT=+KC anim
	convert blueplane.png -modulate 100,100,160 redplane.png

redplane-lr.png blueplane-lr.png: blueplane-lr.pov plane.inc
	$(MAKE) NAME=blueplane-lr WD=48 HT=48 FRAMES=64 OPT=-KC BLUR=1 anim
	convert blueplane-lr.png -modulate 100,100,160 redplane-lr.png

bomb.png: bomb.pov 
	$(MAKE) NAME=bomb WD=16 HT=16 FRAMES=64 anim

bombexplosion.png: bombexplosion.pov
	$(MAKE) NAME=bombexplosion WD=48 HT=48 FRAMES=32 anim

bonus-catch.png: bonus-catch.pov
	$(MAKE) NAME=bonus-catch WD=32 HT=32 FRAMES=16 OPT=+KC anim

bonus-machine.png: bonus-machine.pov 
	$(MAKE) NAME=bonus-machine WD=48 HT=32 FRAMES=16 anim

bonus-ring.png: bonus-ring.pov
	$(MAKE) NAME=bonus-ring WD=32 HT=32 FRAMES=8 OPT=+KC anim

cannon-left.png: cannon-left.pov cannon.inc
	$(MAKE) NAME=cannon-left WD=64 HT=48 FRAMES=16 anim

cannon-right.png: cannon-right.pov cannon.inc
	$(MAKE) NAME=cannon-right WD=64 HT=48 FRAMES=16 anim

cannon-turn-lr.png: cannon-turn-lr.pov cannon.inc
	$(MAKE) NAME=cannon-turn-lr WD=64 HT=48 FRAMES=32 BLUR=1 anim

cannon-turn-rl.png: cannon-turn-rl.pov cannon.inc
	$(MAKE) NAME=cannon-turn-rl WD=64 HT=48 FRAMES=32 BLUR=1 anim

direxpl.png: direxpl.pov
	$(MAKE) NAME=direxpl WD=24 HT=24 FRAMES=256 OPT=+KF32 anim

draco.png: draco.pov
	$(MAKE) NAME=draco WD=96 HT=72 FRAMES=32 BLUR=1 anim

energymeter.png: energymeter.pov
	$(MAKE) NAME=energymeter WD=12 HT=64 FRAMES=16 anim

fire.png: fire.pov
	$(MAKE) NAME=fire WD=24 HT=24 FRAMES=16 OPT=+KC anim

firebomb.png: firebomb.pov
	$(MAKE) NAME=firebomb WD=48 HT=48 FRAMES=32 anim

fuel.png: fuel.pov
	$(MAKE) NAME=fuel WD=24 HT=16 FRAMES=16 anim

gauge.png: gauge.pov
	$(MAKE) NAME=gauge WD=48 HT=48 FRAMES=64 anim

hippo.png: hippo.pov
	$(MAKE) NAME=hippo WD=48 HT=48 FRAMES=64 OPT=+KC anim

hangar.png: hangar.pov
	$(MAKE) NAME=hangar WD=100 HT=64 FRAMES=8 anim

largeflame.png: largeflame.pov
	$(MAKE) NAME=largeflame WD=64 HT=64 FRAMES=24 anim

ldust-dissolve.png: ldust-dissolve.pov dust.inc
	$(MAKE) NAME=ldust-dissolve WD=32 HT=32 FRAMES=8 anim

ldust-grow.png: ldust-grow.pov dust.inc
	$(MAKE) NAME=ldust-grow WD=32 HT=32 FRAMES=8 anim

letter.png: letter.pov
	$(MAKE) NAME=letter WD=16 HT=16 FRAMES=16 anim OPT=+KC BLUR=1

mailbasket.png: mailbasket.pov
	$(MAKE) NAME=mailbasket WD=24 HT=24 FRAMES=8 anim

mark.png: mark.pov
	$(MAKE) NAME=mark WD=24 HT=24 FRAMES=16 OPT=+KC anim

missile.png: missile.pov
	$(MAKE) NAME=missile WD=24 HT=24 FRAMES=64 OPT=+KC anim

box.png: box.pov
	$(MAKE) NAME=box WD=48 HT=48 FRAMES=64 OPT=+KC anim

pine-wave.png: pine-wave.pov
	$(MAKE) NAME=pine-wave WD=24 HT=32 FRAMES=64 anim OPT=+KC BLUR=1

pingu-left.png: pingu-left.pov pingu.inc
	$(MAKE) NAME=pingu-left WD=16 HT=16 FRAMES=8 anim

pingu-right.png: pingu-right.pov pingu.inc
	$(MAKE) NAME=pingu-right WD=16 HT=16 FRAMES=8 anim

pingu-turn.png: pingu-turn.pov pingu.inc
	$(MAKE) NAME=pingu-turn WD=16 HT=16 FRAMES=8 anim

sdust-dissolve.png: sdust-dissolve.pov dust.inc
	$(MAKE) NAME=sdust-dissolve WD=12 HT=12 FRAMES=8 anim

sdust-grow.png: sdust-grow.pov dust.inc
	$(MAKE) NAME=sdust-grow WD=12 HT=12 FRAMES=8 anim

whitesmoke.png smoke.png: smoke.pov
	$(MAKE) NAME=smoke WD=16 HT=16 FRAMES=16 anim
	convert smoke.png -negate whitesmoke.png

spikeball.png: spikeball.pov 
	$(MAKE) NAME=spikeball WD=16 HT=16 FRAMES=32 anim

ufo.png: ufo.pov
	$(MAKE) NAME=ufo WD=48 HT=48 FRAMES=64 OPT=+KC anim

waves.png: waves.pov
	$(MAKE) NAME=waves WD=256 HT=32 FRAMES=16 OPT=+KC anim

zeppelin-turn-lr.png: zeppelin-turn-lr.pov zeppelin.inc
	$(MAKE) NAME=zeppelin-turn-lr WD=96 HT=64 FRAMES=32 BLUR=1 anim

zeppelin-turn-rl.png: zeppelin-turn-rl.pov zeppelin.inc
	$(MAKE) NAME=zeppelin-turn-rl WD=96 HT=64 FRAMES=32 BLUR=1 anim

zeppelin-wreck.png: zeppelin-wreck.pov
	$(MAKE) NAME=zeppelin-wreck WD=96 HT=64 FRAMES=6 anim

# --------------------- stilleben ----------------------------

balloon-float.png: balloon-float.pov
	$(POVRAYP) +I$< +W32 +H32

bonus.png:
	$(MAKE) -C bonus

bullet.png: bullet.pov
	$(POVRAYP) +I$< +W3 +H3

cannonball.png: cannonball.pov
	$(POVRAYP) +I$< +W8 +H8

cloud.png: cloud.pov
	$(POVRAYP) +I$< +W240 +H120

cloud_lph.png: cloud_lph.pov
	$(POVRAYFAST) +I$< +W240 +H120

cloud_lph2.png: cloud_lph2.pov
	$(POVRAYFAST) +I$< +W240 +H120

man.png: man.pov parachute.inc
	$(POVRAYP) +I$< +W48 +H48

mailbag.png: mailbag.pov
	$(POVRAYP) +I$< +W16 +H16

pine.png: pine.pov
	$(POVRAYP) +I$< +W24 +H32

tie.png: tie.pov
	$(POVRAYP) +I$< +W48 +H48

zeppelin-left.png: zeppelin-left.pov zeppelin.inc
	$(POVRAYP) +I$< +W96 +H64

zeppelin-right.png: zeppelin-right.pov zeppelin.inc
	$(POVRAYP) +I$< +W96 +H64

# --------------- other stuff ---------------------------

levels:
	$(MAKE) -C levels

# --------------------------------------------------------

endif # NAME

INSTALLDIR = ../share/graphics/sprites

install: all
	# make a dated backup of the old sprites dir
	if [ -d $(INSTALLDIR) ]; then \
		newdir=$(INSTALLDIR)-$$(date -I); \
		if [ \! -d $$newdir ]; then \
			mv -v $(INSTALLDIR) $$newdir; \
		fi; \
	fi
	mkdir -p $(INSTALLDIR)
	cp -vau $(PNG) $(INSTALLDIR)/
	$(MAKE) -C levels install

clean:
	$(RM) *~ *.bak

distclean: clean
	$(RM) *.png
	$(MAKE) -C bonus distclean
	$(MAKE) -C levels distclean
	$(MAKE) -C background distclean
