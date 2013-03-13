/*
 * Everything related to the world itself - maps, levels, generators, whatnot.
 *
 * Copyright (C) 2013
 *
 */

#ifndef _WORLD_H
#define _WORLD_H

#include "libtcod.hpp"
#include "actor.h"
#include "common.h"

#define AREA_MAX_X 90
#define AREA_MAX_Y 60

#define MAX_AREAS 12

enum cell_type {
        nothing = 0,
        wall,
        floor,
        door_open,
        door_closed,
};

/*class MyCallback: public ITCODBspCallback {
        public:
                bool visitNode(TCODBsp *node, void *userData) {
                        printf("node pos %dx%d size %dx%d level %d\n",node->x,node->y,node->w,node->h,node->level);
                        return true;
                }
};*/

class Cell {
        private:
                cell_type    type;
                int          flags;
                TCODColor    fg, bg;
                bool         visible;
                char         c;
        protected:
        public:
                Cell();
                ~Cell();
                void set_wall();
                void set_floor();
                void set_color(TCODColor fg, TCODColor bg);
                void set_door_closed();
                void set_door_open();
                void set_visibility(bool b);
                void draw(int x, int y);
                void draw(int x, int y, TCODColor fg, TCODColor bg);
                bool is_walkable();
                bool is_visible();
                bool is_transparent();
                cell_type get_type();
                Actor *inhabitant;
};

class Area {
        private:
                TCODBsp *bsp;
                //Cell cell[AREA_MAX_X][AREA_MAX_Y];
        protected:
        public:
                Area();
                ~Area();
                void generate();
                direction generate_starting_room();
                void horizontal_line(int y);
                void horizontal_line(int x, int y, int x2);
                void vertical_line(int x);
                void vertical_line(int x, int y, int y2);
                void frame();
                void build_tcodmap();
                bool cell_is_visible(int x, int y);
                void make_room(int x1, int y1, int x2, int y2);
                void make_door(int x, int y, bool open);
                void set_all_visible();
                void set_all_invisible();
                void update_visibility();

                Cell    **cell;
                TCODMap *tcodmap;
                bool lights_on;

                friend class Cell;
};

class World {
        private:
                Area *area;
                int current_area;
        protected:
        public:
                World();
                ~World();
                const char *get_cell_type(int x, int y);
                bool is_walkable(int x, int y);
                bool is_closed_door(int x, int y);
                bool is_open_door(int x, int y);
                void open_door(int x, int y);
                void close_door(int x, int y);
                bool close_nearest_door(Actor *actor);

                void draw_map();
                void draw_cell(int x, int y);
                void draw_cell(coord_t co);
                void draw_cell(int x, int y, TCODColor fg, TCODColor bg);
                void update_fov();
                coord_t get_random_walkable_cell();
                void set_inhabitant(Actor *actor);
                void clear_inhabitant(coord_t co);

                Area *a;

                friend class Cell;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
