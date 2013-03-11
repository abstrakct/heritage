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
        health = 0;
        setchar('@');
        setxy(PLAYER_STARTX, PLAYER_STARTY);
        setprevxy(getxy());
}

Player::~Player()
{
        dbg("Goodbye, cruel world.");
}
// vim: fdm=syntax guifont=Terminus\ 8
