/*
 * Roguelike Engine in C++
 *
 * Everything related to the world itself - maps, levels, generators, whatnot.
 *
 */

using namespace std;

#include <iostream>

#include "world.h"
#include "common.h"

Cell::Cell()
{
        type = floor;
        flags = 0;
        bg = TCODColor::black;
        fg = TCODColor::white;
        visible = true;
}

Cell::~Cell()
{
}

bool Cell::is_passable()
{
        switch(this->type) {
                case floor:
                case door_open:
                        return true;
                case wall:
                case door_closed:
                case nothing:
                        return false;
        }

        return false;
}

bool Cell::is_visible()
{
        return this->visible;
}

Area::Area()
{
        cell = new Cell*[AREA_MAX_X];

        for(int i = 0; i < AREA_MAX_X; ++i)
                cell[i] = new Cell[AREA_MAX_Y];
}

Area::~Area()
{
        for(int i = 0; i < AREA_MAX_Y; ++i)
                delete [] cell[i];

        delete [] cell;
}

World::World()
{
        area = new Area[MAX_AREAS];
        current_area = 0;
}

World::~World()
{
        delete [] area;
}

bool World::is_passable(int x, int y)
{
        return area[current_area].cell[x][y].is_passable();
}



// vim: fdm=syntax guifont=Terminus\ 8
