SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LIBS := $(shell sdl-config --libs)

IDENTICALINC_DIR = ../src/include
IDENTICALLIB_DIR = ../lib

CPP = gcc
HDRS = -I$(IDENTICALINC_DIR)
DEFS = -DSDLLIB
LDFLAGS = -L$(IDENTICALLIB_DIR) -lidentical-sdl $(SDL_LIBS)

CPPFLAGS = $(HDRS) $(DEFS) $(SDL_CFLAGS)

BASICEXM_OBJ = basicexm.o
BASICEXM_EXE = basicexm

TEXTEXM_OBJ = textexm.o
TEXTEXM_EXE = textexm

all: $(BASICEXM_EXE) $(TEXTEXM_EXE)

$(BASICEXM_EXE): $(BASICEXM_OBJ)
	$(CPP) -o $(BASICEXM_EXE) $(BASICEXM_OBJ) $(LDFLAGS)

$(TEXTEXM_EXE): $(TEXTEXM_OBJ)
	$(CPP) -o $(TEXTEXM_EXE) $(TEXTEXM_OBJ) $(LDFLAGS)

%.o: %.c
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f $(BASICEXM_OBJ) $(TEXTEXM_OBJ) $(BASICEXM_EXE) $(TEXTEXM_EXE)

