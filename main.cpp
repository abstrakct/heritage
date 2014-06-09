/*
 * Roguelike Engine in C++
 */

using namespace std;

#include <iostream>
#include <string>
#include <vector>

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
#include "item.h"

Game *game;
Display *display;
Player *player;
NPC *npc;
World *world;
SoundEngine *audio;

/* 
 * The actual item definitions.
 * Reading them from a text file would be nicer, but would also require more work.
 */
struct item_definition item_definitions[] = {
    // Weapons
    { "knife",       ')', it_weapon,   IF_WIELDABLE, 10,  0, 30 },
    // Clothing
    { "jacket",      '[', it_clothing, IF_WEARABLE,  10,  3, 10 },
    { "fancy pants", '[', it_clothing, IF_WEARABLE,   6,  2, 10 },
    { "gloves",      '[', it_clothing, IF_WEARABLE,   2,  1, 10 },
    // Tools and other things
    { "key",         '?', it_key,      0,             0,  0,  5 },
};

signed int ability_modifier(int ab)
{
    return ((ab / 2) - 5);
}

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
    delete display;
}

void init_npcs()
{
    npc = new NPC[12];

    for(int i=0; i < 12; ++i) {
        npc[i].setxy(world->get_random_walkable_cell(floor_1));
        npc[i].setprevxy(npc[i].getxy());
        npc[i].area = world->get_current_area();
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

    world->generate_stairs();

    world->current_area = (int)floor_1;
    world->a = &world->area[(int)floor_1];
}

void init_player()
{
    for(int i=0;i<80;++i)
        cout << " " << endl;

    player->create();

    player->setxy(world->get_random_walkable_cell(floor_1));
    player->setprevxy(player->getxy());
    player->area = world->get_current_area();
    world->set_inhabitant(player);
}

void init_item_definitions()
{
    Item *i;

    for(size_t x = 0; x < (sizeof(item_definitions) / sizeof(struct item_definition)); x++) {
        i = new Item(item_definitions[x]);
        game->itemdef.push_back(*i);
        delete i;
    }
}

int main(int argc, char **argv)
{
    //unsigned int seed = time(0);
    //srand(seed);

    game = new Game;
    audio = new SoundEngine;
    world = new World;
    player = new Player;

    init_item_definitions();
    init_areas();
    init_player();
    init_npcs();

    display = new Display;
    audio->initialize();
    audio->load_all_files();

    //audio->play_music(SOUND_MUSIC_HOUSE_OF_LEAVES);
    //audio->play_sound(SOUND_EFFECT_STORM01, 0);

    display->message("");     // "kickstart" the messaging system!
    game->loop();

    clean_up();
    return 0;
}

// vim: fdm=syntax guifont=Terminus\ 8
