SDL_CFLAGS := $(shell sdl-config --cflags)

CC = gcc
CFLAGS = -O -DSDLLIB -Isrc/include $(SDL_CFLAGS)
AR = ar
ARFLAGS = rc

OBJ_DIR = obj/

COM_OBJ = igrgeometry.o igrimage.o igrtext.o igrtim.o igrpalette.o \
	igrbasics.o ikbbuffer.o itimer.o

SDL_OBJ = $(addprefix $(OBJ_DIR), $(COM_OBJ))
SDL_LIB = lib/libidentical-sdl.a
TARGET = $(SDL_LIB)

all: $(SDL_LIB)

$(SDL_LIB): $(SDL_OBJ)
	if test ! -d lib; then mkdir -p lib; fi
	$(AR) $(ARFLAGS) $(SDL_LIB) $(SDL_OBJ)

$(OBJ_DIR)%.o: src/%.c
	if test ! -d $(OBJ_DIR); then mkdir -p $(OBJ_DIR); fi
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(SDL_OBJ) $(SDL_LIB)

# dependencies
obj/igrbasics.o: src/include/istdlib.h src/include/igrbasics.h
obj/ikbbuffer.o: src/include/ikbbuffer.h
obj/itimer.o: src/include/istdlib.h src/include/itimer.h
obj/igrgeometry.o: \
	src/include/istdlib.h src/include/igrbasics.h \
	src/include/igrgeometry.h
obj/igrimage.o: \
	src/include/istdlib.h src/include/igrbasics.h src/include/igrimage.h
obj/igrtext.o: src/include/istdlib.h \
	src/include/igrbasics.h src/include/igrtext.h

