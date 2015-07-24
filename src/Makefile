# Note that here are development default settings, higher level
# Makefile overrides these with release settings
USE_SOUND ?= 0
OBJ_DEBUG ?= 0
READLINE ?= 1
EFENCE ?= 1
JOY_DEBUG ?= 0
# You can disable/enable these like this:
# 'make clean; make USE_SOUND=1 EFENCE=0'

# many warnings that gcc supports
WARN := -Wmissing-prototypes -Wstrict-prototypes -Wsign-compare \
  -Wbad-function-cast -Wcast-qual -Wcast-align -Wunused -Wshadow \
  -Wpointer-arith -Wwrite-strings -Wall -Werror
# Problems with -Werror: -std=c99 -pedantic -Wunreachable-code -Wconversion

# initial compilation flags
CPPFLAGS := -Icore -Iengine -Isprites -Isupport
CFLAGS   := -g -O2 -ffast-math $(WARN)

# set some variables to allow compilation under windows
ifeq ($(OS),Windows_NT)
  CC      := gcc
  CFLAGS  += -Dmain=SDL_main
  LDFLAGS := -lmingw32 -mwindows -lSDLmain -lSDL -lSDL_image
  FIND    := e:/gnu/find.exe
else
  CFLAGS  += $(shell sdl-config --cflags)
  CPPFLAGS += $(shell sdl-config --cflags | sed 's/[-][^I][^ ][^ ]* *//g')
  LDFLAGS := $(shell sdl-config --libs) -lSDL_image -lm
  FIND    := find
endif

# handle compilation options
ifeq ($(USE_SOUND),1)
  CPPFLAGS += -DUSE_SOUND
  LDFLAGS += -lSDL_mixer
endif
ifeq ($(OBJ_DEBUG),1)
  CPPFLAGS += -DOBJ_DEBUG
endif
ifeq ($(READLINE),1)
  CPPFLAGS += -DREADLINE
  LDFLAGS += -lreadline
endif
ifeq ($(EFENCE),1)
  LDFLAGS += -lefence
endif
ifeq ($(JOY_DEBUG),1)
  CPPFLAGS += -DDEBUG_JOYSTICK
endif

SRC:= $(wildcard core/*.c engine/*.c sprites/*.c support/*.c tests/*.c)
OBJ:= $(SRC:.c=.o)

SPRITESRC:= $(wildcard sprites/*.c)


airstrike: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ) $(LDFLAGS)

# Use the rule below when coding new sprite types. Only needed for developers
sprites/sprites_autogen.h: $(SPRITESRC)
	cd sprites && ../../utils/genspritelist.sh *.c

clean:
	$(RM) $(shell $(FIND) . -name '*.[od]' -or -name '*~' -or -name '*.bb' -or -name '*.bbg') TAGS dep airstrike

dep: $(SRC)
	gcc $(CPPFLAGS) -MM $^ | sed -r -e 's/(.*\.o):[ ]([^ ]*)/\2: \2/g' | sed -e 's/\.c:/\.o:/g' > dep

TAGS: $(SRC)
	etags $(shell $(FIND) . -name '*.[ch]')

-include dep
