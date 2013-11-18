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
        }

        if(type > 25 && type <= 50) {
                clear_goal();
        }
        
        if(type > 50 && type <= 70) {
                set_goal(this->area->stairs_up);
        }
        
        if(type > 70 && type <= 90) {
                set_goal(this->area->stairs_down);
        }

        if(type > 90 && type <= 97) {
                if(enemy) {
                        //display->message("%s has decided to not kill %s!", this->getname(), this->enemy->getname());
                        enemy = NULL;
                }
                clear_goal();
        }

        if(type > 97) {
                int i = ri(0,12);
                if(i == 12) {
                        if(player->area == this->area) {   // to avoid 3D pathfinding, only attack stuff on one's own level/floor/area
                                enemy = player;
                                set_goal(player->getxy());
                        }
                        //display->message("%s has decided to kill YOU!!!!", this->getname());
                } else {
                        while(!npc[i].is_alive())
                                i = ri(0,11);
                        if(npc[i].area == this->area) {
                                set_goal(npc[i].getxy());
                                enemy = &npc[i];
                        }
                        //display->message("%s has decided to kill %s!", this->getname(), npc[i].getname());
                }
        }
}

void NPC::use_stairs()
{
        if(this->area->cell[this->getx()][this->gety()].get_type() == stairs_up) {
                world->clear_inhabitant(this->area, this->getxy());
                //display->message("%s is moving up some stairs from area %d!", this->getname(), this->area_id);
                int a = (int) this->area_id;
                a++;
                this->area_id = (area_id_type) a;
                this->area = &world->area[this->area_id];
                //display->message("area id is now %d", this->area_id);
                world->set_inhabitant(this);
        } else if(world->area[world->current_area].cell[this->getx()][this->gety()].get_type() == stairs_down) {
                world->clear_inhabitant(this->area, this->getxy());
                //display->message("%s is moving down some stairs from area %d!", this->getname(), this->area_id);
                int a = (int) this->area_id;
                a--;
                this->area_id = (area_id_type) a;
                this->area = &world->area[this->area_id];
                //display->message("area id is now %d", this->area_id);
                world->set_inhabitant(this);
        } 
}

/*
 * This is actually the standard AI, at least for NPC characters
 */
void NPC::path_ai()
{
        if(!has_goal) {
                coord_t c = this->getxy();
                if(world->get_cell_type(this->area, c) == stairs_up || world->get_cell_type(this->area, c) == stairs_down) {
                        this->use_stairs();
                        has_goal = false;
                }
                set_random_goal();
        } else {
                int chance = 60;
                if(enemy) {
                        set_goal(enemy->getxy());
                        chance = 80;
                }
                // Let's walk the path!
                int d;
                int x, y;

                d = ri(1, 100);

                if(d < chance) {                                          // walk, or hang around?
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
