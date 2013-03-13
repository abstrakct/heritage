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
#include "actor.h"
#include "player.h"

extern Player *player;
extern World *world;

class MyCallback : public ITCODBspCallback
{
        public :
                bool visitNode(TCODBsp *node, void *userData)
                {
                        //dbg("trying to make room from %d,%d to %d,%d", node->x, node->y, node->x+node->w, node->y+node->h);
                        //dbg("making room: x=%d y=%d w=%d h=%d position=%d horizontal=%s", node->x, node->y, node->w, node->h, node->position, node->horizontal ? "true" : "false");

                        world->a->make_room(node->x, node->y, node->x + node->w-2, node->y + node->h-2);
                        //dbg("made room from %d,%d to %d,%d", node->x, node->y, node->x+node->w-2, node->y+node->h-2);
                        int x1 = node->x;
                        int y1 = node->y;
                        int x2 = node->x + node->w - 2;
                        int y2 = node->y + node->h - 2;

                        if(node->horizontal) {
                                int ypos = node->y + ri(1, node->h);
                                while(ypos >= AREA_MAX_Y-2)
                                        --ypos;

                                world->a->make_door(x1, ri(y1, y2), false);
                                world->a->make_door(x2, ri(y1, y2), false);
                                //world->a->make_door(node->x, ypos, false);
                                //world->a->make_door(node->x+node->w - ri(2, node->w-2), ypos, false);
                        } else {
                                int xpos = node->x + ri(1, node->w);
                                while(xpos >= AREA_MAX_X-2)
                                        --xpos;
                                
                                world->a->make_door(ri(x1, x2), y1, false);
                                world->a->make_door(ri(x1, x2), y2, false);

                                //world->a->make_door(xpos, node->y, false);
                                //world->a->make_door(xpos, node->y + node->h, false);
                        }

                        return true;
                }
};

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

bool Cell::is_walkable()
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

        return false;
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
        this->fg = TCODColor::grey;
        this->bg = TCODColor::black;
        c = '+';
}

void Cell::set_door_open()
{
        this->type = door_open;
        this->fg = TCODColor::grey;
        this->bg = TCODColor::black;
        c = '\\';
}

void Cell::set_visibility(bool b)
{
        this->visible = b;
}

void Cell::draw(int x, int y)
{
        display->putmap(x, y, this->c, this->fg, this->bg);
}

void Cell::draw(int x, int y, TCODColor fg, TCODColor bg)
{
        display->putmap(x, y, this->c, fg, bg);
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
        bsp = new TCODBsp(1, 1, AREA_MAX_X, AREA_MAX_Y);
        lights_on = true;
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
                        tcodmap->setProperties(x, y, cell[x][y].is_transparent(), cell[x][y].is_walkable());
                }
        }
}

void Area::set_all_invisible()
{
        int x, y;

        for(x = 0; x < AREA_MAX_X; ++x) {
                for(y = 0; y < AREA_MAX_Y; ++y) {
                        cell[x][y].set_visibility(false);
                }
        }
}

void Area::set_all_visible()
{
        int x, y;

        for(x = 0; x < AREA_MAX_X; ++x) {
                for(y = 0; y < AREA_MAX_Y; ++y) {
                        this->tcodmap->setProperties(x, y, true, cell[x][y].is_walkable());
                }
        }
}

void Area::update_visibility()
{
        int x, y;

        for(x = 0; x < AREA_MAX_X; ++x) {
                for(y = 0; y < AREA_MAX_Y; ++y) {
                        if(this->tcodmap->isInFov(x, y))
                                cell[x][y].set_visibility(true);
                        else
                                cell[x][y].set_visibility(false);
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
        //direction d;

        this->frame();

        // let's try to generate a house floor!

        //d = generate_starting_room();

        /*switch(d) {
                case north:
                        generate_room_above(*/

        bsp->splitRecursive(NULL, 5, 3, 3, 1.5f, 1.5f);
        bsp->traversePreOrder(new MyCallback(), NULL);

        this->build_tcodmap();
        lights_on = false;
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
        //dbg("Making door at %d,%d (%s)", x, y, open ? "open" : "closed");

        if(cell[x][y].get_type() != wall)
                return;
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
        //return cell[x][y].is_visible();
}



/*
 * class: World
 */
World::World()
{
        area = new Area[MAX_AREAS];
        current_area = 0;
        a = &area[current_area];
}

World::~World()
{
        delete [] area;
}

bool World::is_walkable(int x, int y)
{
        return a->cell[x][y].is_walkable();
}

bool World::is_closed_door(int x, int y)
{
        if(a->cell[x][y].get_type() == door_closed)
                return true;
        else
                return false;
}

bool World::is_open_door(int x, int y)
{
        if(a->cell[x][y].get_type() == door_open)
                return true;
        else
                return false;
}

void World::open_door(int x, int y)
{
        a->cell[x][y].set_door_open();
        a->build_tcodmap();
}

void World::close_door(int x, int y)
{
        a->cell[x][y].set_door_closed();
        a->build_tcodmap();
}

bool World::close_nearest_door(Actor *actor)
{
        // Close the door nearest to acto, start at northwest.
        // Returns true if successfully closed a door.

        int x, y, dx, dy;

        x = actor->getx();
        y = actor->gety();
        for(dx=-1; dx <= 1; dx++) {
                for(dy=-1; dy <= 1; dy++) {
                        if(is_open_door(x+dx, y+dy)) {
                                close_door(x+dx, y+dy);
                                return true;
                        }
                }
        }

        return false;
}

void World::draw_map()
{
        int i, j;

        for(i = 1; i < AREA_MAX_X-1; ++i) {
                for(j = 1; j < AREA_MAX_Y-1; ++j) {
                        if(this->a->cell_is_visible(i, j)) {
                                draw_cell(i, j);
                        } else {
                                if(!a->lights_on)
                                        draw_cell(i, j, TCODColor::black, TCODColor::black); 
                        }
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

void World::draw_cell(int x, int y, TCODColor fg, TCODColor bg)
{
        a->cell[x][y].draw(x, y, fg, bg);
}

void World::update_fov()
{
        coord_t co;

        co = player->getxy();
        a->tcodmap->computeFov(co.x, co.y, 9, true, FOV_BASIC);
}

coord_t World::get_random_walkable_cell()
{
        coord_t co;

        co.x = ri(1, AREA_MAX_X);
        co.y = ri(1, AREA_MAX_Y);
        while(!a->cell[co.x][co.y].is_walkable()) {
                co.x = ri(1, AREA_MAX_X);
                co.y = ri(1, AREA_MAX_Y);
        }

        return co;
}

        




// vim: fdm=syntax guifont=Terminus\ 8
