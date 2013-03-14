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

enum floor_id_type {
        cellar_6 = 0,
        cellar_5,
        cellar_4,
        cellar_3,
        cellar_2,
        cellar_1,
        floor_1,
        floor_2,
        floor_3,
        floor_4,
        floor_5,
        floor_6,
};


enum cell_type {
        nothing = 0,
        wall,
        floor,
        door_open,
        door_closed,
        stairs_up,
        stairs_down,
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
                void set_visibility(bool b);
                void set_stairs_up();
                void set_stairs_down();
                void setfg(TCODColor color) { fg = color; };
                void setbg(TCODColor color) { bg = color; };
                TCODColor getfg() { return fg; };

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
                floor_id_type id;
                //Cell cell[AREA_MAX_X][AREA_MAX_Y];
        protected:
        public:
                Area();
                ~Area();
                void generate(floor_id_type identifier);
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
                void make_stairs_up(coord_t co);
                void make_stairs_down(coord_t co);
                void set_all_visible();
                void set_all_invisible();
                void update_visibility();
                void set_id(floor_id_type identifier) { id = identifier; };
                floor_id_type get_id() { return id; };
                const char *get_area_name();

                Cell    **cell;
                TCODMap *tcodmap;
                bool lights_on;
                coord_t stairs_up;
                coord_t stairs_down;

                friend class Cell;
};

class World {
        private:
        protected:
        public:
                World();
                ~World();
                const char *get_cell_type(int x, int y);
                cell_type get_cell_type(coord_t co);
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
                coord_t get_random_walkable_cell(floor_id_type id);
                void set_inhabitant(Actor *actor);
                void clear_inhabitant(coord_t co);

                int current_area;
                Area *area;
                Area *a;

                friend class Cell;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
