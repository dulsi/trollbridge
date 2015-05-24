IDENTICALINC_DIR=../identical/src/include
IDENTICALLIB_DIR=../identical/lib

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

CXX = g++
CXXFLAGS = -Wall -g -DSDLLIB -I$(IDENTICALINC_DIR) $(SDL_CFLAGS)
DLLFLAGS = -fPIC -I.
LIBS = -L$(IDENTICALLIB_DIR) -lidentical-sdl -ldl $(SDL_LDFLAGS)

DLL_DIR = gamedll

TROLL_OBJ = definition.o character.o file.o game.o main.o nescont.o \
	screen.o sprite.o thing.o monster.o item.o carrieditem.o \
	projectile.o secret.o exit.o
TROLL_EXE = troll

DLL_OBJ = $(DLL_DIR)/dllmain.o $(DLL_DIR)/items.o $(DLL_DIR)/people.o \
	$(DLL_DIR)/door.o $(DLL_DIR)/scorpion.o $(DLL_DIR)/bug.o \
	$(DLL_DIR)/block.o $(DLL_DIR)/undead.o $(DLL_DIR)/closedoor.o \
	$(DLL_DIR)/cobra.o $(DLL_DIR)/blast.o $(DLL_DIR)/explosion.o \
	$(DLL_DIR)/healer.o $(DLL_DIR)/jumper.o $(DLL_DIR)/octoid.o \
	$(DLL_DIR)/turtle.o
TROLL_DLL = troll.so

ifndef prefix
	prefix=
endif
ifndef LIB_DIR
	LIB_DIR=lib
endif
BINDIR=${prefix}/usr/bin
LIBDIR=${prefix}/usr/$(LIB_DIR)/troll
DATADIR=${prefix}/usr/share/troll

all: $(TROLL_EXE) $(TROLL_DLL)

$(TROLL_EXE): $(TROLL_OBJ)
	$(CXX) -Wl,-E -o $(TROLL_EXE) $(TROLL_OBJ) $(LIBS)

$(TROLL_DLL): $(DLL_OBJ)
	$(CXX) -shared -o $(TROLL_DLL) $(DLL_OBJ)

$(DLL_DIR)/%.o: $(DLL_DIR)/%.C
	$(CXX) $(DLLFLAGS) $(CXXFLAGS) -DLIBRARY_DIR=\"${LIBDIR}\" -DDATA_DIR=\"${DATADIR}\" -c -o $@ $<

%.o: %.C
	$(CXX) $(CXXFLAGS) -DLIBRARY_DIR=\"${LIBDIR}\" -DDATA_DIR=\"${DATADIR}\" -c -o $@ $<

clean:
	rm -f $(TROLL_OBJ) $(DLL_OBJ) $(TROLL_EXE) $(TROLL_DLL)

install:
	mkdir -p $(BINDIR)
	cp troll $(BINDIR)
	mkdir -p $(LIBDIR)
	cp troll.so $(LIBDIR)
	mkdir -p $(DATADIR)
	cp trolltitle.pcx ../data/troll.def ../data/troll.spr ../data/troll.pal ../data/levels/*/*.trb $(DATADIR)
	mkdir -p ${prefix}/usr/include/troll
	cp *.h ${prefix}/usr/include/troll
	if test ! -d ${prefix}/usr/share/icons/hicolor; then mkdir -p ${prefix}/usr/share/icons/hicolor; fi
	cp -R ../icons/* ${prefix}/usr/share/icons/hicolor/
	desktop-file-install --dir=${prefix}/usr/share/applications ../troll.desktop

# dependencies
character.o game.o main.o screen.o thing.o item.o monster.o carrieditem.o \
	projectile.o secret.o exit.o $(DLL_OBJ): troll.h trollconst.h file.h \
	sprite.h
file.o: file.h
sprite.o: file.h sprite.h
nescont.o: nescont.h
$(DLL_DIR)/dllmain.o: $(DLL_DIR)/items.h $(DLL_DIR)/people.h $(DLL_DIR)/bug.h \
	$(DLL_DIR)/scorpion.h $(DLL_DIR)/block.h $(DLL_DIR)/healer.h
$(DLL_DIR)/items.o: $(DLL_DIR)/items.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/people.o: $(DLL_DIR)/people.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/door.o: $(DLL_DIR)/items.h $(DLL_DIR)/dllconst.h $(DLL_DIR)/door.h
$(DLL_DIR)/scorpion.o: $(DLL_DIR)/scorpion.h $(DLL_DIR)/dllconst.h \
	$(DLL_DIR)/explosion.h
$(DLL_DIR)/bug.o: $(DLL_DIR)/bug.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/block.o: $(DLL_DIR)/block.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/undead.o: $(DLL_DIR)/undead.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/closedoor.o: $(DLL_DIR)/closedoor.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/cobra.o: $(DLL_DIR)/cobra.h $(DLL_DIR)/blast.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/blast.o: $(DLL_DIR)/blast.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/explosion.o: $(DLL_DIR)/explosion.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/healer.o: $(DLL_DIR)/healer.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/jumper.o: $(DLL_DIR)/jumper.h $(DLL_DIR)/dllconst.h
$(DLL_DIR)/octoid.o: $(DLL_DIR)/octoid.h $(DLL_DIR)/dllconst.h

