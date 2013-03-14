/*
 * Roguelike Engine in C++
 *
 * Player specific stuff.
 *
 */

using namespace std;

#include <iostream>

#include "actor.h"
#include "player.h"
#include "common.h"
#include "display.h"
#include "world.h"

extern World   *world;
extern Player  *player;
extern Display *display;

Player::Player()
{
        //body = mind = soul = 10;
        //sanity = 100;
        //fear = 0;
        //health = 100;

        setstat(sBody, 10);
        setstat(sMind, 10);
        setstat(sSoul, 10);
        setstat(sSanity, 100);
        setstat(sFear, 0);
        setstat(sHealth, 100);
        setchar('@');
        setname("Boris Hoffman");
        setcolors(TCODColor::darkAzure, TCODColor::black);
        setfovradius(9);
}

Player::~Player()
{
        //dbg("Goodbye, cruel world.");
}

void Player::look()
{
        cell_type cell;

        cell = world->get_cell_type(player->getxy());
        switch(cell) {
                case stairs_up:
                        display->message("There is a staircase leading up here.");
                        break;
                case stairs_down:
                        display->message("There is a staircase leading down here.");
                        break;
                default:
                        break;
        }
}

// vim: fdm=syntax guifont=Terminus\ 8
