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
        c = ' ';
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

void Cell::set_wall()
{
        this->type = wall;
        c = '#';
}

void Cell::set_floor()
{
        this->type = floor;
        c = '.';
}

void Cell::draw(int x, int y)
{
        display->putmap(x, y, this->c, this->fg, this->bg);
}

void Cell::set_color(TCODColor fg, TCODColor bg)
{
        this->fg = fg;
        this->bg = bg;
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

void Area::generate()
{
       int i, j;

       for(i = 0; i < AREA_MAX_X; ++i) {
               for(j = 0; j < AREA_MAX_Y; ++j) {
                       if(fiftyfifty()) {
                               cell[i][j].set_wall();
                               cell[i][j].set_color(TCODColor::red, TCODColor::black);
                       }
               }
       }
}

World::World()
{
        area = new Area[MAX_AREAS];
        current_area = 0;
        area[current_area].generate();
        a = &area[current_area];
}

World::~World()
{
        delete [] area;
}

bool World::is_passable(int x, int y)
{
        return area[current_area].cell[x][y].is_passable();
}

void World::draw_map()
{
        int i, j;

        Area *a = &area[current_area];

        for(i = 1; i < AREA_MAX_X-1; ++i) {
                for(j = 1; j < AREA_MAX_Y-1; ++j) {
                        a->cell[i][j].draw(i, j);
                }
        }               
}

void World::draw_cell(int x, int y)
{
        a->cell[x][y].draw(x, y);
}

void World::draw_cell(coord_t co)
{
        a->cell[co.x][co.y].draw(co.x, co.y);
}



// vim: fdm=syntax guifont=Terminus\ 8
