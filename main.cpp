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

int dice(int num, int sides, signed int modifier)
{
        int i, result;

        result = modifier;
        for(i=0;i<num;i++) {
                result += 1 + (rand() % sides);
        }

        return result;
}

int ri(int a, int b) 
{
        int result;
        result = (a + (rand() % (b-a+1)));

        return result;
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
        unsigned int seed = time(0);
        //srand(seed);

        game = new Game;
        npc = new NPC;
        audio = new SoundEngine;
        world = new World;
        player = new Player;

        player->setxy(world->get_random_walkable_cell());
        player->setprevxy(player->getxy());

        world->a->generate();

        audio->initialize();
        audio->load_all_files();
        
        //audio->play_music(2);
        //audio->play_sound(0, 5000);

        game->loop();
        
        clean_up();
        return 0;
}

// vim: fdm=syntax guifont=Terminus\ 8
