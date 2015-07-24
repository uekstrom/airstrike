
POVFLAGS = +FN +DTC -P +A0.3 -J +KC +UA +W32 +H32 -L.. -L../include

POV = airballoon bird bomb clone cloud energy fuel hippo love missile parachute ufo zeppelin
PNG = $(patsubst %, bonus-%.png, $(POV))
BONUS = ../bonus.png

all: $(BONUS) msg_heart.png

msg_heart.png: bonus-love.pov
	povray $(POVFLAGS) +I$< +W16 +H16 +O$@

$(BONUS): $(PNG)
	convert +append bonus-*.png $@

%.png: %.pov
	povray $(POVFLAGS) +I$<

clean:
	$(RM) *~ *.bak

distclean: clean
	$(RM) *.png
	
