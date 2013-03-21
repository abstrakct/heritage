/*
 * Roguelike Engine in C++
 *
 * Everything related to the world itself - maps, levels, generators, whatnot.
 *
 */

using namespace std;

#include <iostream>

#include "actor.h"
#include "world.h"
#include "common.h"
#include "player.h"

extern Player *player;
extern World *world;

const char *area_name[] = {
        "Cellar, level 6",
        "Cellar, level 5",
        "Cellar, level 4",
        "Cellar, level 3",
        "Cellar, level 2",
        "Cellar, level 1",
        "1st floor",
        "2nd floor",
        "3rd floor",
        "4th floor",
        "5th floor",
        "6th floor",
};

class MyCallback : public ITCODBspCallback
{
        public :
                bool visitNode(TCODBsp *node, void *userData)
                {
                        world->a->make_room(node->x, node->y, node->x + node->w-2, node->y + node->h-2);

                        if(node->horizontal) {
                                int x1 = node->x - 2;
                                int y1 = node->y - 2;
                                int x2 = node->x + node->w - 2;
                                int y2 = node->y + node->h - 2;
                                world->a->make_door(x1, ri(y1, y2), false);
                                world->a->make_door(x2, ri(y1, y2), false);
                        } else {
                                int x1 = node->x;
                                int y1 = node->y;
                                int x2 = node->x + node->w - 2;
                                int y2 = node->y + node->h - 2;
                                world->a->make_door(ri(x1, x2), y1, false);
                                world->a->make_door(ri(x1, x2), y2, false);
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
        activated = false;
        this->set_floor();
        inhabitant = NULL;
}

Cell::~Cell()
{
}

bool Cell::is_walkable()
{
        /*if(this->inhabitant) {
                if(this->inhabitant != player)
                        display->message("%s is in your way!", this->inhabitant->getname());
                return false;
        }*/

        switch(this->type) {
                case floor:
                case door_open:
                case door_closed:
                case stairs_up:
                case stairs_down:
                case cell_corpse:
                case cell_bookcase:
                case cell_table:
                case cell_chair:
                case cell_coffin:
                case cell_candle:
                        return true;
                case wall:
                case nothing:
                        return false;
                default:
                        return false;
        }

        return false;
}

bool Cell::is_transparent()
{
        switch(this->type) {
                case floor:
                case door_open:
                case stairs_up:
                case stairs_down:
                case cell_corpse:
                case cell_bookcase:
                case cell_table:
                case cell_chair:
                case cell_coffin:
                case cell_candle:
                        return true;
                case wall:
                case door_closed:
                case nothing:
                        return false;
                default:
                        return true;
        }
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

void Cell::set_stairs_up()
{
        this->type = stairs_up;
        this->fg = TCODColor::yellow;
        this->bg = TCODColor::black;
        c = '<';
}

void Cell::set_stairs_down()
{
        this->type = stairs_down;
        this->fg = TCODColor::yellow;
        this->bg = TCODColor::black;
        c = '>';
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

void Cell::set_corpse(Actor *who)
{
        this->type = cell_corpse;
        this->fg = TCODColor::black;
        this->bg = TCODColor::red;
        c = 0x0F;
        corpse = who;
}

void Cell::set_bookcase()
{
        this->type = cell_bookcase;
        this->fg = TCODColor::lightestAzure;
        this->bg = TCODColor::black;
        c = 'H';
}

void Cell::set_chair()
{
        this->type = cell_chair;
        this->fg = TCODColor::azure;
        this->bg = TCODColor::black;
        c = 'h';
}

void Cell::set_table()
{
        this->type = cell_table;
        this->fg = TCODColor::lightFlame;
        this->bg = TCODColor::black;
        c = 'T';
}

void Cell::set_visibility(bool b)
{
        this->visible = b;
}

void Cell::draw(int x, int y)
{
        if(inhabitant) {
                if(inhabitant->alive) {
                        if(inhabitant->area == world->a) {
                                inhabitant->draw();
                        }
                }
        } else {
                display->putmap(x, y, this->c, this->fg, this->bg);
        }
        display->touch();
}

void Cell::draw(int x, int y, TCODColor fore, TCODColor back)
{
        if(inhabitant) {
                if(inhabitant->alive) {
                        if(inhabitant->area == world->a) {
                                inhabitant->draw(fore, back);
                        }
                }
        } else {
                display->putmap(x, y, this->c, fore, back);
        }
        display->touch();
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

void Cell::activate_bookcase()
{
        if(activated) {
                display->message("You find nothing more of interest in this bookcase.");
                break;
        }

        display->message("You spend a little while browsing the books in this bookcase, looking for something interesting.");
        i = dice(1, 20, 0);
        if(i <= 10) {
                book = ri(1, 5);
                switch(book) {
                        case 1:
                                display->message("An old book titled \"Unlocking Your Brain's True Potential - Vol. I\" catches your eye. It's written by Dr. Evan Hoffman.");
                                x = dice(1, 20, 0);
                                if(x <= player->getstat(sMind)) {
                                        display->message("You open the book at \"Chapter XIII - Telekinetics and related subjects\" and read for a while.");
                                        display->message("Congratulations! You can now employ your mind to cause minor destruction!");
                                        player->special[2] = special_mindblast;
                                } else {
                                        display->message("You flip randomly through the book, but the medical terminology goes straight over your head. You put the book back on the shelf.");
                                }
                                display->message(" ");
                        case 2:
                                display->message("You come across a book called \"How To Fight - A Manual\". It looks interesting.");
                                x = dice(1, 20, 0);
                                if(x <= player->getstat(sMind)) {
                                        display->message("You read through the book, picking up a few useful fighting techniques.");
                                        display->message("Congratulations! You can now use a special move in physical combat!");
                                        player->special[3] = special_powerfist;
                                } else {
                                        display->message("You flip through the book, but see nothing you don't already know.");
                                }
                                display->message(" ");
                                break;
                        case 3:
                                display->message("Your gaze stops at a book titled \"Physical Excercises for Modern Men and Women\".");
                                x = dice(1, 20, 0);
                                if(x <= player->getstat(sMind)) {
                                        display->message("Inside the book are pictures and descriptions of various physical excercises. You take notice of some of these.");
                                        display->message("Congratulations! You feel more prepared for physical combat.");
                                        player->incstat(sBody, 1);
                                } else {
                                        display->message("Inside the book are pictures and descriptions of various physical excercises.");
                                        display->message("You discard the book. Books are no substitute for real physical excercise!");
                                }
                                display->message(" ");
                                break;
                        case 4:
                                display->message("You find a small book called \"Mysteries of the Soul\", written by Marvin E. A. Edeef.");
                                x = dice(1, 20, 0);
                                if(x <= player->getstat(sMind)) {
                                        display->message("As you flip through the book you come to a sudden realization about the soul and your own spirituality.");
                                        display->message("Congratulations! You can now use some of the powers of your soul to help you make it through the night!");
                                        player->special[1] = special_soulcrush;
                                } else {
                                        display->message("You flip through the book, quickly concluding that it's just New-Age mumbo jumbo.");
                                }
                                display->message(" ");
                                break;
                        default:
                                display->message("The only book which looks interesting is unfortunately written in some language you don't even recognize.");
                                display->message(" ");
                                break;
                }
        } else {
                display->message("After a while, you conclude that there's nothing here that interests you.");
                display->message(" ");
        }
        activated = true;
}

void Cell::activate()
{
        int i, book, x;
        switch(this->type) {
                case cell_bookcase:
                        this->activate_bookcase();
                        break;
                default:
                        break;
        }
        display->touch();
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
        for(int i = 0; i < AREA_MAX_X; ++i)
                delete [] cell[i];

        delete [] cell;
        delete tcodmap;
        delete bsp;
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
        player->setfovradius(0);
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

void Area::generate(area_id_type identifier)
{
        MyCallback *callback = new MyCallback;
        world->a = &world->area[(int)identifier];
        set_id(identifier);
        bsp->splitRecursive(NULL, 5, 3, 3, 1.5f, 1.5f);
        bsp->traversePreOrder(callback, NULL);
        this->frame();

        if(identifier != floor_6) {
                coord_t co = world->get_random_floor_cell(identifier);
                make_stairs_up(co);
                world->area[(int)identifier+1].make_stairs_down(co);

                // Make two sets of stairs! TODO: implement correctly ;)
                /*
                co = world->get_random_floor_cell(identifier);
                make_stairs_up(co);
                world->area[(int)identifier+1].make_stairs_down(co);*/
        }

        this->build_tcodmap();
        lights_on = false;

        place_furniture(identifier);
        delete callback;
}

void Area::place_furniture(area_id_type identifier)
{
        // bookcases
        for(int i = 0; i < 10; ++i) {
                if(fiftyfifty()) {
                        coord_t c = world->get_random_walkable_cell(identifier);
                        this->cell[c.x][c.y].set_bookcase();
                }
        }
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

        if(x <= 1)
                return;
        if(y <= 1)
                return;
        if(cell[x][y].get_type() != wall)
                return;

        if(open)
                cell[x][y].set_door_open();
        else
                cell[x][y].set_door_closed();
}

void Area::make_stairs_up(coord_t co)
{
        cell[co.x][co.y].set_stairs_up();
        stairs_up = co;
}

void Area::make_stairs_down(coord_t co)
{
        cell[co.x][co.y].set_stairs_down();
        stairs_down = co;
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

const char *Area::get_area_name()
{
        if(id >= 0 && id <= MAX_AREAS)
                return area_name[(int) id];
        else
                return NULL;
}

bool Area::is_walkable(int x, int y)
{
        return this->cell[x][y].is_walkable();
}



/*
 * class: World
 */
World::World()
{
        area = new Area[MAX_AREAS];
        current_area = 0;
        //a = &area[current_area];
}

World::~World()
{
        delete [] area;
}

bool World::is_walkable(Area *where, int x, int y)
{
        return where->cell[x][y].is_walkable();
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
        // Close the door nearest to actor, start at northwest.
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
                        if(area[current_area].cell_is_visible(i, j)) {
                                draw_cell(i, j);
                        } else {
                                if(!area[current_area].lights_on) {
                                        draw_cell(i, j, TCODColor::black, TCODColor::black); 
                                }
                        }
                }
        }               
        display->touch();
}

void World::draw_cell(int x, int y)
{
       // a->cell[x][y].draw(x, y);
        if(a->cell[x][y].inhabitant) {
                if(a->cell[x][y].inhabitant->alive) {
                        if(a->cell[x][y].inhabitant->area == world->a) {
                                a->cell[x][y].inhabitant->draw();
                        }
                }
        } else {
                display->putmap(x, y, a->cell[x][y].c, a->cell[x][y].fg, a->cell[x][y].bg);
        }
}

void World::draw_cell(coord_t co)
{
        a->cell[co.x][co.y].draw(co.x, co.y);
}

void World::draw_cell(int x, int y, TCODColor fg, TCODColor bg)
{
        //a->cell[x][y].draw(x, y, fg, bg);
        if(a->cell[x][y].inhabitant) {
                if(a->cell[x][y].inhabitant->alive) {
                        if(a->cell[x][y].inhabitant->area == world->a) {
                                a->cell[x][y].inhabitant->draw(fg, bg);
                        }
                }
        } else {
                display->putmap(x, y, a->cell[x][y].c, fg, bg);
        }
}

void World::update_fov()
{
        coord_t co;

        co = player->getxy();
        a->tcodmap->computeFov(co.x, co.y, player->getfovradius(), true, FOV_BASIC);
}

coord_t World::get_random_walkable_cell(area_id_type id)
{
        coord_t co;

again:
        co.x = ri(1, AREA_MAX_X-2);
        co.y = ri(1, AREA_MAX_Y-2);
        while(!area[id].cell[co.x][co.y].is_walkable()) {
                co.x = ri(1, AREA_MAX_X-2);
                co.y = ri(1, AREA_MAX_Y-2);
        }

        if(area[id].cell[co.x][co.y].is_walkable())
                return co;
        else
                goto again;
}

coord_t World::get_random_floor_cell(area_id_type id)
{
        coord_t co;

again:
        co.x = ri(1, AREA_MAX_X-2);
        co.y = ri(1, AREA_MAX_Y-2);
        while(area[id].cell[co.x][co.y].get_type() != floor) {
                co.x = ri(1, AREA_MAX_X-2);
                co.y = ri(1, AREA_MAX_Y-2);
        }

        if(area[id].cell[co.x][co.y].get_type() == floor)
                return co;
        else
                goto again;
}

void World::set_inhabitant(Actor *actor)
{
        actor->area->cell[actor->getx()][actor->gety()].inhabitant = actor;
        actor->area_id = actor->area->get_id();
}

void World::clear_inhabitant(coord_t co)
{
        a->cell[co.x][co.y].inhabitant = NULL;
        if(a->cell_is_visible(co.x, co.y))
                a->cell[co.x][co.y].draw(co.x, co.y);
        else
                a->cell[co.x][co.y].draw(co.x, co.y, TCODColor::black, TCODColor::black);
}

const char *World::get_cell_type(int x, int y)
{
        switch(a->cell[x][y].get_type()) {
                case floor: return "floor"; break;
                case wall: return "wall"; break;
                case nothing: return "nothing"; break;
                case door_open: return "open door"; break;
                case door_closed: return "closed door"; break;
                default: return "unknown"; break;
        }
}

cell_type World::get_cell_type(coord_t co)
{
        return world->area[current_area].cell[co.x][co.y].get_type();
}
        




// vim: fdm=syntax guifont=Terminus\ 8
