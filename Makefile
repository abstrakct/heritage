CXX = g++ 
CXXFLAGS = -Wall -I. -Iinclude -g -ggdb
DEBUGFLAGS = -g -ggdb
LIBS = -lm -ltcodxx -lSDL -lSDL_mixer -Llib -Wl,-rpath=lib
LDFLAGS=$(LIBS)     #,-rpath=lib 
DEFINES =
 
SOURCES = main.cpp actor.cpp display.cpp game.cpp command.cpp player.cpp sound.cpp npc.cpp world.cpp
HEADERS = actor.h display.h game.h command.h debug.h player.h sound.h npc.h world.h
OBJS    = main.o actor.o display.o game.o command.o player.o sound.o npc.o world.o

heritage: $(OBJS)
	$(CXX) $(DEFINES) $(LDFLAGS) -o $@ $(OBJS)

heritage-debug: $(OBJS)
	$(CXX) $(DEBUGFLAGS) $(DEFINES) $(LDFLAGS) -o $@ $(OBJS)

.: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEFINES) -o $@ $(SOURCES)

clean:
	rm -f *.o heritage 

depend:
	$(CXX) $(CXXFLAGS) -MM *.cpp > .deps

all: heritage

include .deps
