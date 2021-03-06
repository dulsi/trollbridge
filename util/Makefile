IDENTICALINC_DIR = ../identical/src/include
IDENTICALLIB_DIR = ../identical/lib

ifndef prefix
	prefix=
endif

SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

TROLLINC_DIR = ../src
TROLLOBJ_DIR = ../src

CXX = g++
CXXFLAGS = -g -I$(TROLLINC_DIR) -I$(IDENTICALINC_DIR) -DSDLLIB $(SDL_CFLAGS)
DLLFLAGS = -fPIC
LIBS = -L$(IDENTICALLIB_DIR) -lidentical-sdl -ldl $(SDL_LDFLAGS) -lboost_filesystem -lboost_system -lSDL2_image

CREATEMAP_OBJ = createmap.o iextra.o
CREATEMAP_EXE = createmap

TEXT2PAL_OBJ = text2pal.o iextra.o $(TROLLOBJ_DIR)/file.o
TEXT2PAL_EXE = text2pal

TEXT2PCX_OBJ = text2pcx.o iextra.o
TEXT2PCX_EXE = text2pcx

PCX2TEXT_OBJ = pcx2text.o iextra.o
PCX2TEXT_EXE = pcx2text

PCX2ZSP_OBJ = pcx2zsp.o
PCX2ZSP_EXE = pcx2zsp

SCREENEDIT_OBJ = screenedit.o trolledit.o $(TROLLOBJ_DIR)/file.o \
	$(TROLLOBJ_DIR)/sprite.o $(TROLLOBJ_DIR)/exit.o
SCREENEDIT_EXE = screenedit

SPLITPCX_OBJ = splitpcx.o iextra.o
SPLITPCX_EXE = splitpcx

all: $(CREATEMAP_EXE) $(TEXT2PAL_EXE) $(TEXT2PCX_EXE) $(PCX2TEXT_EXE) \
	$(IMAGE2TEXT_EXE) $(SCREENEDIT_EXE) $(PCX2ZSP_EXE) $(SPLITPCX_EXE)

$(CREATEMAP_EXE): $(CREATEMAP_OBJ)
	$(CXX) -o $(CREATEMAP_EXE) $(CREATEMAP_OBJ) $(LIBS)

$(TEXT2PAL_EXE): $(TEXT2PAL_OBJ)
	$(CXX) -o $(TEXT2PAL_EXE) $(TEXT2PAL_OBJ) $(LIBS)

$(TEXT2PCX_EXE): $(TEXT2PCX_OBJ)
	$(CXX) -o $(TEXT2PCX_EXE) $(TEXT2PCX_OBJ) $(LIBS)

$(PCX2TEXT_EXE): $(PCX2TEXT_OBJ)
	$(CXX) -o $(PCX2TEXT_EXE) $(PCX2TEXT_OBJ) $(LIBS)

$(PCX2ZSP_EXE): $(PCX2ZSP_OBJ)
	$(CXX) -o $(PCX2ZSP_EXE) $(PCX2ZSP_OBJ) $(LIBS)

$(SCREENEDIT_EXE): $(SCREENEDIT_OBJ)
	$(CXX) $(CXXFLAGS) -o $(SCREENEDIT_EXE) $(SCREENEDIT_OBJ) $(LIBS)

$(SPLITPCX_EXE): $(SPLITPCX_OBJ)
	$(CXX) -o $(SPLITPCX_EXE) $(SPLITPCX_OBJ) $(LIBS)

%.o: %.C
	$(CXX) $(CXXFLAGS) -c -o $@ $<
%.o: %.c
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TEXT2PAL_EXE) $(CREATEMAP_EXE)
	rm -f $(TEXT2PCX_EXE) $(PCX2TEXT_EXE) $(PCX2ZSP_EXE)
	rm -f $(SCREENEDIT_EXE) $(SPLITPCX_EXE)

install:
	mkdir -p ${prefix}/usr/bin
	cp $(CREATEMAP_EXE) $(TEXT2PAL_EXE) $(TEXT2PCX_EXE) ${prefix}/usr/bin
	cp $(PCX2TEXT_EXE) $(SCREENEDIT_EXE) $(PCX2ZSP_EXE) ${prefix}/usr/bin
	cp $(SPLITPCX_EXE) ${prefix}/usr/bin
	mkdir -p ${prefix}/usr/share/man/man1
	cp ../man/man1/* ${prefix}/usr/share/man/man1

# dependencies
createmap.o: iextra.h
pcx2text.o: iextra.h
text2pal.o: iextra.h $(TROLLINC_DIR)/file.h
text2pcx.o: iextra.h
iextra.o: iextra.h
screenedit.o: trolledit.h
trolledit.o: trolledit.h
$(TROLLOBJ_DIR)/file.o: $(TROLLINC_DIR)/file.h
$(TROLLOBJ_DIR)/sprite.o: $(TROLLINC_DIR)/file.h $(TROLLINC_DIR)/sprite.h

