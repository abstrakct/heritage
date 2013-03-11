/*
 * Everything related to the world itself - maps, levels, generators, whatnot.
 *
 * Copyright (C) 2013
 *
 */

#ifndef _WORLD_H
#define _WORLD_H

#include "libtcod.hpp"
#include "common.h"

#define AREA_MAX_X 90
#define AREA_MAX_Y 60

#define MAX_AREAS 12

enum cell_type {
        nothing,
        wall,
        floor,
        door_open,
        door_closed,
};

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
                void draw(int x, int y);
                bool is_passable();
                bool is_visible();
                bool is_transparent();
                cell_type get_type();
};

class Area {
        private:
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

                Cell    **cell;
                TCODMap *tcodmap;

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
                bool is_passable(int x, int y);
                bool is_closed_door(int x, int y);
                void open_door(int x, int y);

                void draw_map();
                void draw_cell(int x, int y);
                void draw_cell(coord_t co);
                void update_fov();

                Area *a;

                friend class Cell;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
