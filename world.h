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
                void draw(int x, int y);
                bool is_passable();
                bool is_visible();
};

class Area {
        private:
                //Cell cell[AREA_MAX_X][AREA_MAX_Y];
        protected:
        public:
                Area();
                ~Area();
                void generate();
                Cell **cell;
};

class World {
        private:
                Area *area;
                int current_area;
        protected:
        public:
                World();
                ~World();
                Area *a;
                bool is_passable(int x, int y);
                void draw_map();
                void draw_cell(int x, int y);
                void draw_cell(coord_t co);
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
