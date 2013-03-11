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
#include "player.h"

extern Player *player;
extern World *world;

/*
 * class: Cell
 */
Cell::Cell()
{
        flags = 0;
        visible = false;
        this->set_floor();
}

Cell::~Cell()
{
}

bool Cell::is_passable()
{
        switch(this->type) {
                case floor:
                case door_open:
                case wall:
                        return true;
                case door_closed:
                case nothing:
                        return false;
        }

        return false;
}

bool Cell::is_transparent()
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

        return true;
}

bool Cell::is_visible()
{
        return this->visible; 
}

void Cell::set_wall()
{
        this->type = wall;
        this->fg = TCODColor::darkerFlame;
        this->bg = TCODColor::black;
        c = '#';
}

void Cell::set_floor()
{
        this->type = floor;
        this->fg = TCODColor::darkerGrey;
        this->bg = TCODColor::black;
        c = '.';
}

void Cell::set_door_closed()
{
        this->type = door_closed;
        this->fg = TCODColor::white;
        this->bg = TCODColor::black;
        c = '+';
}

void Cell::set_door_open()
{
        this->type = door_open;
        this->fg = TCODColor::white;
        this->bg = TCODColor::black;
        c = '\\';
}

void Cell::draw(int x, int y)
{
        if(world->a->cell_is_visible(x, y))
                display->putmap(x, y, this->c, this->fg, this->bg);
}

void Cell::set_color(TCODColor fg, TCODColor bg)
{
        this->fg = fg;
        this->bg = bg;
}

cell_type Cell::get_type()
{
        return this->type;
}



/*
 * class: Area
 */
Area::Area()
{
        cell = new Cell*[AREA_MAX_X];

        for(int i = 0; i < AREA_MAX_X; ++i)
                cell[i] = new Cell[AREA_MAX_Y];

        tcodmap = new TCODMap(AREA_MAX_X, AREA_MAX_Y);
}

Area::~Area()
{
        for(int i = 0; i < AREA_MAX_Y; ++i)
                delete [] cell[i];

        delete [] cell;
}

void Area::build_tcodmap()
{
        int x, y;

        for(x = 0; x < AREA_MAX_X; ++x) {
                for(y = 0; y < AREA_MAX_Y; ++y) {
                        tcodmap->setProperties(x, y, cell[x][y].is_transparent(), cell[x][y].is_passable());
                }
        }
}

direction Area::generate_starting_room()
{
        int x1, x2, y1, y2;
        direction d;

        x1 = PLAYER_STARTX - 3;
        x2 = PLAYER_STARTX + 3;
        y1 = PLAYER_STARTY + 1;
        y2 = PLAYER_STARTY - 6;
        make_room(x1, y1, x2, y2);
        
        d = (direction) d(1, 4);

        if(d == north || d == south) {
                int dx, dy;
                dx = ri(x1+1, x2-1);
                dy = y2;
                make_door(dx, dy, false);
        }
        /*if(d == south) {
                int dx, dy;
                dx = ri(x1, x2);
                dy = y1;
                make_door(dx, dy, false);
        }*/
        if(d == west) {
                int dx, dy;
                dx = x1;
                dy = ri(y1-2, y2+1);
                make_door(dx, dy, false);
        }
        if(d == east) {
                int dx, dy;
                dx = x2;
                dy = ri(y1-2, y2+1);
                make_door(dx, dy, false);
        }

        return d;
}

void Area::generate()
{
        direction d;

        this->frame();

        // let's try to generate a house floor!

        int rooms_vertical = ri(4,8);
        int rooms_horizontal = ri(4,8);
        int x = ri(5, 8);

        for(int i = 0; i < rooms_vertical; ++i) {
                int maxsize = AREA_MAX_X / rooms_vertical;
                vertical_line(x);

                x += ri(3,maxsize);
                if(x >= AREA_MAX_X)
                        x = AREA_MAX_X - 3;
        }

        int y = ri(5,8);
        for(int i = 0; i < rooms_horizontal; ++i) {
                int maxsize = AREA_MAX_X / rooms_vertical;
                horizontal_line(y);

                y += ri(3,maxsize);
                if(y >= AREA_MAX_Y)
                        y = AREA_MAX_Y - 3;
        }

        d = generate_starting_room();

        /*switch(d) {
                case north:
                        generate_room_above(*/

        

        this->build_tcodmap();
}

void Area::frame()
{
        horizontal_line(1);
        horizontal_line(AREA_MAX_Y - 2);
        vertical_line(1);
        vertical_line(AREA_MAX_X - 2);
}

void Area::make_room(int x1, int y1, int x2, int y2)
{
        horizontal_line(x1, y1, x2);
        horizontal_line(x1, y2, x2);
        vertical_line(x1, y1, y2);
        vertical_line(x2, y1, y2);
}

void Area::make_door(int x, int y, bool open)
{
        dbg("Making door at %d,%d (%s)", x, y, open ? "open" : "closed");
        if(open)
                cell[x][y].set_door_open();
        else
                cell[x][y].set_door_closed();
}

void Area::horizontal_line(int y)
{
        for(int i = 0; i < AREA_MAX_X; ++i)
                cell[i][y].set_wall();
}

void Area::horizontal_line(int x, int y, int x2)
{
        if(x2 > x) {
                for(int j = x; j <= x2; ++j)
                        cell[j][y].set_wall();
        } else {
                for(int j = x; j >= x2; --j)
                        cell[j][y].set_wall();
        }
}

void Area::vertical_line(int x, int y, int y2)
{
        if(y2 > y) {
                for(int j = y; j <= y2; ++j)
                        cell[x][j].set_wall();
        } else if(y2 < y) {
                for(int j = y; j > y2; --j)
                        cell[x][j].set_wall();
        }
}

void Area::vertical_line(int x)
{
        for(int j = 0; j < AREA_MAX_Y; ++j)
                cell[x][j].set_wall();
}

bool Area::cell_is_visible(int x, int y)
{
        return tcodmap->isInFov(x, y);
}



/*
 * class: World
 */
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
        return a->cell[x][y].is_passable();
}

bool World::is_closed_door(int x, int y)
{
        if(a->cell[x][y].get_type() == door_closed)
                return true;
        else
                return false;
}

void World::open_door(int x, int y)
{
        dbg("opening door at %d,%d", x, y);
        a->cell[x][y].set_door_open();
        a->build_tcodmap();
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

void World::update_fov()
{
        coord_t co;

        co = player->getxy();
        a->tcodmap->computeFov(co.x, co.y, 0, true, FOV_BASIC);
}



// vim: fdm=syntax guifont=Terminus\ 8
