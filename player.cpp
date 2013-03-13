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


// vim: fdm=syntax guifont=Terminus\ 8
