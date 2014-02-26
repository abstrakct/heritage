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
#include "player.h"
#include "common.h"
#include "world.h"

extern Display *display;
extern World *world;
extern NPC *npc;
extern Player *player;

#include "npc-names.h"

#define MALE_GIVEN_NAMES   1217
#define FEMALE_GIVEN_NAMES 4273
#define LASTNAMES 125

NPC::NPC()
{
    TCODColor fg, bg;

    setstat(sHealth, 100);
    setstat(sSanity, 100);
    setstat(sFear, 0);
    setstat(sMind, ri(1,20));
    setstat(sBody, ri(1,20));
    setstat(sSoul, ri(1,20));
    setchar('@');

    fg = display->get_random_color();
    bg = TCODColor::black; // display->get_random_color();
    setcolors(fg, bg);

    setai(AI_RANDOM);
    path = new TCODPath(world->a->tcodmap, 1.0f);
    has_goal = false;
    goal_type = no_goal;
    enemy = NULL;

    setgender(ri(0, 1));
    generate_name();
}

NPC::~NPC()
{
    delete path;
}

void NPC::generate_name()
{
    char n[64];
    if(is_male()) {
        int i = ri(0, MALE_GIVEN_NAMES);
        strcpy(n, malegivenname[i]);
    } else {
        int i = ri(0, FEMALE_GIVEN_NAMES);
        strcpy(n, femalegivenname[i]);
    }
    strcat(n, " ");
    int i = ri(0, LASTNAMES);
    strcat(n, lastnames[i]);

    setname(n);
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

    type = ri(1,100);
    if(type <= 25) {    // goal is random location on the level.
        set_goal(world->get_random_walkable_cell(this->area_id));
        //display->message("%s has a goal: to walk to %d,%d", this->getname(), this->goal.x, this->goal.y);
        this->goal_type = move_random;
    }

    if(type > 25 && type <= 50) {  // abandon goal, set a new goal
        if(has_goal) {
            if(this->pass_roll(sSoul)) {
                display->message("%s has abandoned goal.", this->getname());
                clear_goal();
                set_random_goal();
            }
        }
    }

    if(type > 50 && type <= 70) {  // move upstairs
        //display->message("%s has a goal: to go upstairs", this->getname());
        set_goal(this->area->stairs_up);
        this->goal_type = move_upstairs;
    }

    if(type > 70 && type <= 90) {  // move downstairs
        //display->message("%s has a goal: to go downstairs", this->getname());
        set_goal(this->area->stairs_down);
        this->goal_type = move_downstairs;
    }

    if(type > 90 && type <= 97) {  // not kill someone
        if(enemy) {
            if(this->enemy == player)
                if(player->pass_roll(sSoul))
                    display->messagec(COLOR_GOOD, "You feel good vibrations.");
#ifdef DEBUG_ON
            display->message("%s has decided to NOT kill %s after all!", this->getname(), this->enemy->getname());
#endif
            clear_goal();
            enemy = NULL;
        }
    }

    if(type > 97) {                // kill someone
        int i = ri(0,12);
        if(i == 12) {
            if(player->area == this->area) {   // to avoid 3D pathfinding, only attack stuff on one's own level/floor/area
                enemy = player;
                set_goal(player->getxy());
                this->goal_type = kill_player;
            }
#ifdef DEBUG_ON
            display->message("%s has decided to kill YOU!!!!", this->getname());
#endif
            if(player->pass_roll(sSoul))
                display->messagec(COLOR_FEAR, "You sense sinister forces at work.");
        } else {
            while(!npc[i].is_alive())
                i = ri(0,11);
            if(npc[i].area == this->area) {
                set_goal(npc[i].getxy());
                enemy = &npc[i];
                this->goal_type = kill_npc;
            }
            display->message("%s has decided to kill %s!", this->getname(), npc[i].getname());
        }
    }
}

void NPC::set_in_combat()
{
    in_combat = true;
    has_goal = true;
}

void NPC::use_stairs()
{
    if(this->area->cell[this->getx()][this->gety()].get_type() == stairs_up) {
        world->clear_inhabitant(this->area, this->getxy());
        int a = (int) this->area_id;
        a++;
        this->area_id = (area_id_type) a;
        this->area = &world->area[this->area_id];
        this->setxy(world->area[this->area_id].stairs_down);
        world->set_inhabitant(this);
        delete path;
        path = new TCODPath(this->area->tcodmap, 1.0f);
    } else if(world->area[world->current_area].cell[this->getx()][this->gety()].get_type() == stairs_down) {
        world->clear_inhabitant(this->area, this->getxy());
        int a = (int) this->area_id;
        a--;
        this->area_id = (area_id_type) a;
        this->area = &world->area[this->area_id];
        this->setxy(world->area[this->area_id].stairs_up);
        world->set_inhabitant(this);
        delete path;
        path = new TCODPath(this->area->tcodmap, 1.0f);
    } 
}

/*
 * This is actually the standard AI, at least for NPC characters
 */
void NPC::path_ai()
{
    coord_t curr = this->getxy();

    if(has_goal) {
        if(this->goal_type == move_upstairs || this->goal_type == move_downstairs) {
            if(world->get_cell_type(this->area, curr) == stairs_up || world->get_cell_type(this->area, curr) == stairs_down) {
                this->use_stairs();
                has_goal = false;
            }
        }
    }

    if(!has_goal) {
        set_random_goal();
    } else {
        int chance = 70;

        if(enemy) {
            if(this->in_combat) {
                if(this->is_next_to(this->enemy)) {
                    attack(this->enemy);
                    return;
                }
            }

            if(this->area == enemy->area) {
                set_goal(enemy->getxy());
                if(this->in_combat)
                    chance = 95;
                else
                    chance = 90;
            } else {
                if(this->in_combat) {
                    bool x = fiftyfifty();
                    if(x) {
                        set_goal(this->area->stairs_down);
                        this->goal_type = move_downstairs;
                    } else {
                        set_goal(this->area->stairs_up);
                        this->goal_type = move_upstairs;
                    }
                } else {
                    has_goal = false;
                    set_random_goal();
                }
            }
        }

        // Let's walk the path!
        int d;
        int x, y;

        d = ri(1, 100);

        path->compute(curr.x, curr.y, goal.x, goal.y);
        if(d < chance) {                                          // walk, or hang around?
            curr = this->getxy();

            if(path->walk(&x, &y, true)) {
                // success
                if(x > curr.x && y == curr.y)
                    move_right();
                if(x < curr.x && y == curr.y)
                    move_left();
                if(x == curr.x && y > curr.y)
                    move_down();
                if(x == curr.x && y < curr.y)
                    move_up();
                if(x > curr.x && y > curr.y)
                    move_se();
                if(x < curr.x && y > curr.y)
                    move_sw();
                if(x < curr.x && y < curr.y)
                    move_nw();
                if(x > curr.x && y < curr.y)
                    move_ne();
            } else {
                // walking the path failed - set new goal.
                has_goal = false;
            }
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
