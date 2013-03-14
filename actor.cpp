/*
 * Generic actor class definition
 *
 * Copyright (C) 2013
 */

using namespace std;
#include "common.h"
#include "actor.h"
#include "display.h"
#include "world.h"

extern World *world;
extern Display *display;

const char *sanitydesc[] = {
        "Member of WBC",              //  0 - 3
        "Completely insane",          //  3 - 10
        "Raving and drooling",        // 11 - 20
        "Nervous breakdown imminent", // 21 - 30
        "You're losing it",           // 31 - 40
        "Average man in the street",  // 41 - 60
        "Less insane than most",      // 61 - 70
        "Still doing great",          // 71 - 80
        "Pretty darn sane",           // 81 - 90 
        "Completely sane",            // 91 - 99
        "Mental health poster child", // 100
};

Actor::Actor()
{
        role = role_unknown;
        c = ' ';
        co.x = co.y = 0;
        alive = true;
}

/*Actor::~Actor()
{
}*/

/*bool Actor::is_player()
{
        if(role == player)
                return true;
        else
                return false;
}*/

bool Actor::is_alive()
{
        if(alive)
                return true;
        else
                return false;
}

void Actor::kill()
{
        alive = false;
}

void Actor::setxy(int x, int y)
{
        co.x = x;
        co.y = y;
}

void Actor::setxy(coord_t newco)
{
        //dbg("setting co to %d,%d (type of cell is %s)", newco.x, newco.y, world->get_cell_type(newco.x, newco.y));
        co = newco;
}

int Actor::getx()
{
        return co.x;
}

int Actor::gety()
{
        return co.y;
}

void Actor::setprevxy(int x, int y)
{
        prev.x = x;
        prev.y = y;
}

void Actor::setprevxy(coord_t newco)
{
        prev = newco;
}

void Actor::setchar(char newc)
{
        c = newc;
}

void Actor::setname(const char *name)
{
        strcpy(this->name, name);
}

void Actor::setcolors(TCODColor fg, TCODColor bg)
{
        this->fg = fg;
        this->bg = bg;
}

char *Actor::getname()
{
        return this->name;
}

coord_t Actor::getxy()
{
        return co;
}

void Actor::draw()
{
        if(world->a->cell_is_visible(co.x, co.y)) {
                if(prev.x != co.x || prev.y != co.y || (prev.x != co.x && prev.y != co.y))
                        world->draw_cell(this->prev);
                display->putmap(this->co.x, this->co.y, this->c, this->fg, this->bg);
                display->touch();
        }
}

void Actor::draw(TCODColor fg, TCODColor bg)
{
        if(world->a->cell_is_visible(co.x, co.y)) {
                if(prev.x != co.x || prev.y != co.y || (prev.x != co.x && prev.y != co.y))
                        world->draw_cell(this->prev);
                display->putmap(this->co.x, this->co.y, this->c, fg, bg);
                display->touch();
        }
}

void Actor::move_left()
{
        if(world->is_closed_door(this->co.x - 1, this->co.y)) {
                world->open_door(this->co.x - 1, this->co.y);
                display->touch();
                return;
        }
        if(world->is_walkable(this->co.x - 1, this->co.y)) {
                prev = co;
                co.x -= 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_right()
{
        if(world->is_closed_door(this->co.x + 1, this->co.y)) {
                world->open_door(this->co.x + 1, this->co.y);
                return;
                display->touch();
        }
        if(world->is_walkable(this->co.x + 1, this->co.y)) {
                prev = co;
                co.x += 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_down()
{
        if(world->is_closed_door(this->co.x, this->co.y + 1)) {
                world->open_door(this->co.x, this->co.y + 1);
                return;
                display->touch();
        }
        if(world->is_walkable(this->co.x, this->co.y + 1)) {
                prev = co;
                co.y += 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_up()
{
        if(world->is_closed_door(this->co.x, this->co.y - 1)) {
                world->open_door(this->co.x, this->co.y - 1);
                display->touch();
                return;
        }
        
        if(world->is_walkable(this->co.x, this->co.y - 1)) {
                prev = co;
                co.y -= 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_nw()
{
        if(world->is_closed_door(this->co.x - 1, this->co.y - 1)) {
                world->open_door(this->co.x - 1, this->co.y - 1);
                display->touch();
                return;
        }
        if(world->is_walkable(this->co.x - 1, this->co.y - 1)) {
                prev = co;
                co.x -= 1;
                co.y -= 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_ne()
{
        if(world->is_closed_door(this->co.x + 1, this->co.y - 1)) {
                world->open_door(this->co.x + 1, this->co.y - 1);
                display->touch();
                return;
        }
        if(world->is_walkable(this->co.x + 1, this->co.y - 1)) {
                prev = co;
                co.x += 1;
                co.y -= 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_sw()
{
        if(world->is_closed_door(this->co.x - 1, this->co.y + 1)) {
                world->open_door(this->co.x - 1, this->co.y + 1);
                display->touch();
                return;
        }
        if(world->is_walkable(this->co.x - 1, this->co.y + 1)) {
                prev = co;
                co.x -= 1;
                co.y += 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}

void Actor::move_se()
{
        if(world->is_closed_door(this->co.x + 1, this->co.y + 1)) {
                world->open_door(this->co.x + 1, this->co.y + 1);
                display->touch();
                return;
        }
        if(world->is_walkable(this->co.x + 1, this->co.y + 1)) {
                prev = co;
                co.x += 1;
                co.y += 1;
                world->clear_inhabitant(prev);
                world->set_inhabitant(this);
                display->touch();
        }
}


const char *Actor::get_sanitydesc()
{
        if(getstat(sSanity) == 100)
                return sanitydesc[10];
        else if(getstat(sSanity) >= 91)
                return sanitydesc[9];
        else if(getstat(sSanity) >= 81)
                return sanitydesc[8];
        else if(getstat(sSanity) >= 71)
                return sanitydesc[7];
        else if(getstat(sSanity) >= 61)
                return sanitydesc[6];
        else if(getstat(sSanity) >= 41)
                return sanitydesc[5];
        else if(getstat(sSanity) >= 31)
                return sanitydesc[4];
        else if(getstat(sSanity) >= 21)
                return sanitydesc[3];
        else if(getstat(sSanity) >= 11)
                return sanitydesc[2];
        else if(getstat(sSanity) >= 3)
                return sanitydesc[1];
        else 
                return sanitydesc[0];
}

int Actor::getstat(enum_stat which)
{
        return this->stat[(int)which];

        /*switch(which) {
                case sMind:
                        return this->mind;
                        break;
                case sBody:
                        return this->body;
                        break;
                case sSoul:
                        return this->soul;
                        break;
                case sSanity:
                        return this->sanity;
                        break;
                case sFear:
                        return this->fear;
                        break;
                case sHealth:
                        return this->health;
                        break;
                default:
                        return 0;
        }*/
}

void Actor::setstat(enum_stat which, int what)
{
        this->stat[which] = what;
}

void Actor::decstat(enum_stat which, int amount)
{
        this->stat[which] -= amount;
}

// vim: fdm=syntax guifont=Terminus\ 8
