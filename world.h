/*
 * Everything related to the world itself - maps, levels, generators, whatnot.
 *
 * Copyright (C) 2013
 *
 */

#ifndef _WORLD_H
#define _WORLD_H

#include "libtcod.hpp"

#define AREA_MAX_X 128
#define AREA_MAX_Y 75

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
        protected:
        public:
                Cell();
                ~Cell();
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
                bool is_passable(int x, int y);
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
