/*
 * Roguelike Engine in C++
 *
 * NPC specific stuff.
 *
 */

using namespace std;

#include <iostream>

#include "actor.h"
#include "npc.h"
#include "common.h"

NPC::NPC()
{
        health = 0;
        setchar('@');
        setxy(20, 30);
}

NPC::~NPC()
{
        dbg("Goodbye, cruel world.");
}
// vim: fdm=syntax guifont=Terminus\ 8

