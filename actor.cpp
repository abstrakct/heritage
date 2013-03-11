/*
 * Generic actor class definition
 *
 * Copyright (C) 2013
 */

using namespace std;
#include "common.h"
#include "actor.h"


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

void Actor::setchar(char newc)
{
        c = newc;
}

void Actor::draw()
{
        display->putmap(this->co.x, this->co.y, this->c);
}

void Actor::move_left()
{
        if(world->is_passable(this->co.x - 1, this->co.y)) {
                prev.x = co.x;
                co.x -= 1;
        }
}

void Actor::move_right()
{
        co.x += 1;
}

void Actor::move_down()
{
        co.y += 1;
}

void Actor::move_up()
{
        co.y -= 1;
}

void Actor::move_nw()
{
        co.x -= 1;
        co.y -= 1;
}

void Actor::move_ne()
{
        co.x += 1;
        co.y -= 1;
}

void Actor::move_sw()
{
        co.x -= 1;
        co.y += 1;
}

void Actor::move_se()
{
        co.x += 1;
        co.y += 1;
}


// vim: fdm=syntax guifont=Terminus\ 8
