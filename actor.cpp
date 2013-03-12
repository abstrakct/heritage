/*
 * Generic actor class definition
 *
 * Copyright (C) 2013
 */

using namespace std;
#include "common.h"
#include "actor.h"
#include "world.h"

extern World *world;

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

coord_t Actor::getxy()
{
        return co;
}

void Actor::draw()
{
        world->draw_cell(this->prev);
        display->putmap(this->co.x, this->co.y, this->c);
}

void Actor::move_left()
{
        if(world->is_closed_door(this->co.x - 1, this->co.y)) {
                world->open_door(this->co.x - 1, this->co.y);
                return;
        }
        if(world->is_walkable(this->co.x - 1, this->co.y)) {
                prev = co;
                co.x -= 1;
        }
}

void Actor::move_right()
{
        if(world->is_closed_door(this->co.x + 1, this->co.y)) {
                world->open_door(this->co.x + 1, this->co.y);
                return;
        }
        if(world->is_walkable(this->co.x + 1, this->co.y)) {
                prev = co;
                co.x += 1;
        }
}

void Actor::move_down()
{
        if(world->is_closed_door(this->co.x, this->co.y + 1)) {
                world->open_door(this->co.x, this->co.y + 1);
                return;
        }
        if(world->is_walkable(this->co.x, this->co.y + 1)) {
                prev = co;
                co.y += 1;
        }
}

void Actor::move_up()
{
        if(world->is_closed_door(this->co.x, this->co.y - 1)) {
                world->open_door(this->co.x, this->co.y - 1);
                return;
        }
        
        if(world->is_walkable(this->co.x, this->co.y - 1)) {
                prev = co;
                co.y -= 1;
        }
}

void Actor::move_nw()
{
        if(world->is_walkable(this->co.x - 1, this->co.y - 1)) {
                prev = co;
                co.x -= 1;
                co.y -= 1;
        }
}

void Actor::move_ne()
{
        if(world->is_walkable(this->co.x + 1, this->co.y - 1)) {
                prev = co;
                co.x += 1;
                co.y -= 1;
        }
}

void Actor::move_sw()
{
        if(world->is_walkable(this->co.x - 1, this->co.y + 1)) {
                prev = co;
                co.x -= 1;
                co.y += 1;
        }
}

void Actor::move_se()
{
        if(world->is_walkable(this->co.x + 1, this->co.y + 1)) {
                prev = co;
                co.x += 1;
                co.y += 1;
        }
}


// vim: fdm=syntax guifont=Terminus\ 8
