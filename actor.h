/*
 * Generic actor class definition
 *
 * Copyright (C) 2013
 *
 */

#ifndef _ACTOR_H
#define _ACTOR_H

#include "common.h"

enum e_role {
        role_unknown = 0,
        role_player,
        role_npc,
        role_enemy,
};

class Actor {
        public:
                Actor();
                virtual ~Actor() { dbg("This is the end...."); };
                virtual bool is_player() = 0;
                bool is_alive();
                void kill();
                void draw();
                coord_t getxy();
                void setxy(int x, int y);
                void setxy(coord_t newco);
                int  getx();
                int  gety();
                void setprevxy(int x, int y);
                void setprevxy(coord_t newco);
                void setchar(char newc);
                void move_left();
                void move_right();
                void move_up();
                void move_down();
                void move_nw();
                void move_ne();
                void move_sw();
                void move_se();
        protected:
        private:
                e_role role;
                bool alive;
                char c;
                coord_t co;
                coord_t prev;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
