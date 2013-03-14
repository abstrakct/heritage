/*
 * Roguelike Engine in C++
 *
 * NPC specific stuff.
 *
 */

using namespace std;

#include <iostream>

#include "libtcod.hpp"
#include "display.h"
#include "actor.h"
#include "npc.h"
#include "common.h"
#include "world.h"

extern Display *display;
extern World *world;

NPC::NPC()
{
        TCODColor fg, bg;

        setstat(sHealth, 100);
        setchar('@');

        fg = display->get_random_color();
        bg = TCODColor::black; // display->get_random_color();
        setcolors(fg, bg);
        setai(AI_RANDOM);
        path = new TCODPath(world->a->tcodmap, 1.0f);
        has_goal = false;
}

NPC::~NPC()
{
        delete path;
}


void NPC::setai(int which)
{
        which_ai = which;
}

void NPC::random_ai()
{
        int choice = ri(1,9);
        switch (choice) {
                case 1:
                        this->move_sw();
                        break;
                case 2:
                        this->move_down();
                        break;
                case 3:
                        this->move_se();
                        break;
                case 4:
                        this->move_left();
                        break;
                case 5:
                        break;
                case 6:
                        this->move_right();
                        break;
                case 7:
                        this->move_nw();
                        break;
                case 8:
                        this->move_up();
                        break;
                case 9:
                        this->move_ne();
                        break;
                default:
                        break;
        }
}

void NPC::set_random_goal()
{
        int type;

        type = ri(1,4);
        if(type == 1) {    // goal is random location on the level.
                set_goal(world->get_random_walkable_cell(this->area_id));
        } else if(type == 2) {
                set_goal(world->a->stairs_up);
        } else if(type == 3) {
                set_goal(world->a->stairs_down);
        } else if(type == 4) {
                set_goal(world->get_random_walkable_cell(this->area_id));
        }
}

void NPC::path_ai()
{
        if(!has_goal) {
                set_random_goal();
        } else {
                // Let's walk the path!
                int x, y;
                coord_t c = this->getxy();
                path->compute(c.x, c.y, goal.x, goal.y);
                if(path->walk(&x, &y, true)) {
                        // success
                        if(x > c.x && y == c.y)
                                move_right();
                        if(x < c.x && y == c.y)
                                move_left();
                        if(x == c.x && y > c.y)
                                move_down();
                        if(x == c.x && y < c.y)
                                move_up();
                        if(x > c.x && y > c.y)
                                move_se();
                        if(x < c.x && y > c.y)
                                move_sw();
                        if(x < c.x && y < c.y)
                                move_nw();
                        if(x > c.x && y < c.y)
                                move_ne();
                } else {
                        // walking the path failed - set new goal.
                        has_goal = false;
                }
        }
}

void NPC::ai()
{
        switch(which_ai) {
                case AI_RANDOM:
                        this->random_ai();
                        break;
                case AI_PATH:
                        this->path_ai();
                        break;
                default:
                        dbg("no ai defined.");
                        break;
        }
}



// vim: fdm=syntax guifont=Terminus\ 8

