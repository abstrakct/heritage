/*
 * Roguelike Engine in C++
 */

using namespace std;

#include <iostream>

#include <unistd.h>
#include "libtcod.hpp"

// #include "actor.h"
#include "command.h"
#include "debug.h"
#include "display.h"
#include "game.h"
#include "player.h"
#include "npc.h"
#include "sound.h"
#include "world.h"

Game *game;
Display *display;
Player *player;
NPC *npc;
World *world;
SoundEngine *audio;

bool fiftyfifty()
{
        int i;

        i = 1 + (rand() % 100);

        if(i <= 50)
                return true;
        else
                return false;
}

void clean_up()
{
        delete audio;
        delete npc;
        delete player;
        delete game;
        delete world;
}

int main(int argc, char **argv)
{
        game = new Game;
        player = new Player;
        npc = new NPC;
        audio = new SoundEngine;
        world = new World;

        srand(time(0));

        //audio->initialize();
        //audio->load_all_files();

        game->loop();
        
        clean_up();
        return 0;
}

// vim: fdm=syntax guifont=Terminus\ 8
