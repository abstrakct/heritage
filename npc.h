/*
 * NPC specific stuff
 *
 * Copyright (C) 2013
 *
 */

#ifndef _NPC_H
#define _NPC_H

#include "libtcod.hpp"
#include "actor.h"

class NPC: public Actor {
        public:
                NPC();
                ~NPC();
                bool is_player() { return false; }
                void setai(int which);
                void ai();
                void set_goal(coord_t c) { goal = c; has_goal = true; };
                void generate_name();

                bool has_goal;
                int which_ai;
                TCODPath *path;
        private:
                void random_ai();
                void path_ai();
                void set_random_goal();

                coord_t goal;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8

