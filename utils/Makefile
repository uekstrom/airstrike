
PROGS = showanim hfsmooth

all: $(PROGS)

showanim: showanim.c
	$(CC) -Wall -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

hfsmooth: hfsmooth.c
	$(CC) -Wall -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

pan: pan.c
	$(CC) -Wall -g -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

hfinter: hfinter.c
	$(CC) -Wall -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

heightfield: heightfield.c
	$(CC) -Wall -g -o $@ $^ `sdl-config --cflags --libs` -lSDL_image

clean:
	rm -f $(PROGS) *~ \#*
