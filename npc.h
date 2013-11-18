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

enum goal_type_e {
        no_goal = 0,
        move_random,
        move_upstairs,
        move_downstairs,
        kill_player,
        kill_npc
};
        

class NPC: public Actor {
        public:
                NPC();
                ~NPC();
                bool is_player() { return false; }
                void setai(int which);
                void ai();
                void set_goal(coord_t c) { goal = c; has_goal = true; };
                void clear_goal() { has_goal = false; goal_type = no_goal; };
                void generate_name();
                void use_stairs();

                bool has_goal;
                goal_type_e goal_type;
                int  which_ai;
                TCODPath *path;
        private:
                void random_ai();
                void path_ai();
                void set_random_goal();

                coord_t goal;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8

