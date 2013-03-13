/*
 * Roguelike Engine in C++
 *
 * NPC specific stuff.
 *
 */

using namespace std;

#include <iostream>

#include "display.h"
#include "actor.h"
#include "npc.h"
#include "common.h"

extern Display *display;

NPC::NPC()
{
        TCODColor fg, bg;

        setstat(sHealth, 100);
        setchar('@');

        fg = display->get_random_color();
        bg = TCODColor::black; // display->get_random_color();
        setcolors(fg, bg);
        setai(AI_RANDOM);
}

NPC::~NPC()
{
        //dbg("Goodbye, cruel world.");
}




// vim: fdm=syntax guifont=Terminus\ 8

