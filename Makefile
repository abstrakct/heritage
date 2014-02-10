CXX = g++ 
CXXFLAGS = -Wall -I. -Iinclude -g -ggdb -DDEBUG_ON
DEBUGFLAGS = -g -ggdb -DDEBUG_ON
LIBS = -lm -ltcodxx -lSDL -lSDL_mixer -Llib -Wl,-rpath=lib
LDFLAGS=$(LIBS)     #,-rpath=lib 
DEFINES = -DDEBUG_ON
 
SOURCES = main.cpp actor.cpp display.cpp game.cpp command.cpp player.cpp sound.cpp npc.cpp world.cpp
HEADERS = actor.h display.h game.h command.h debug.h player.h sound.h npc.h world.h
OBJS    = main.o actor.o display.o game.o command.o player.o sound.o npc.o world.o

heritage: $(OBJS)
	$(CXX) -DDEBUG_ON $(DEFINES) $(LDFLAGS) -o $@ $(OBJS)

heritage-debug: $(OBJS)
	$(CXX) -DDEBUG_ON $(DEBUGFLAGS) $(DEFINES) $(LDFLAGS) -o $@ $(OBJS)

.: $(SOURCES) $(HEADERS)
	$(CXX) -DDEBUG_ON $(CXXFLAGS) $(DEFINES) -o $@ $(SOURCES)

clean:
	rm -f *.o heritage 

depend:
	$(CXX) $(CXXFLAGS) -MM *.cpp > .deps

all: heritage

include .deps
