/*
 * Roguelike Engine in C++
 */

using namespace std;

#include <iostream>

#include <unistd.h>
#include "libtcod.hpp"

#include "actor.h"
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
        delete player;
        delete world;
        delete audio;
        delete [] npc;
        delete game;
}

void init_npcs()
{
        npc = new NPC[12];

        for(int i=0; i < 12; ++i) {
                npc[i].setxy(world->get_random_walkable_cell(floor_1));
                npc[i].setprevxy(npc[i].getxy());
                world->set_inhabitant(&npc[i]);
                npc[i].setai(AI_PATH);
                
                //dbg("NPC %d set x,y = %d,%d", i, npc[i].getx(), npc[i].gety());
        }
}

void init_areas()
{
        world->area[floor_1].generate(floor_1);
        world->area[floor_2].generate(floor_2);
        world->area[floor_3].generate(floor_3);
        world->area[floor_4].generate(floor_4);
        world->area[floor_5].generate(floor_5);
        world->area[floor_6].generate(floor_6);
        world->area[cellar_1].generate(cellar_1);
        world->area[cellar_2].generate(cellar_2);
        world->area[cellar_4].generate(cellar_3);
        world->area[cellar_5].generate(cellar_4);
        world->area[cellar_6].generate(cellar_5);
        world->area[cellar_6].generate(cellar_6);
        
        world->current_area = (int)floor_1;
        world->a = &world->area[(int)floor_1];
}

void init_player()
{
        player->setxy(world->get_random_walkable_cell(floor_1));
        player->setprevxy(player->getxy());
        world->set_inhabitant(player);
}

int main(int argc, char **argv)
{
        //unsigned int seed = time(0);
        //srand(seed);

        game = new Game;
        audio = new SoundEngine;
        world = new World;
        player = new Player;

        init_areas();
        init_player();
        init_npcs();

        audio->initialize();
        audio->load_all_files();
        
        //audio->play_music(2);
        //audio->play_sound(0, 5000);

        display->message("");     // "kickstart" the messaging system!
        game->loop();
        
        clean_up();
        return 0;
}

// vim: fdm=syntax guifont=Terminus\ 8
