/*
 * Player specific stuff
 *
 * Copyright (C) 2013
 *
 */

#ifndef _PLAYER_H
#define _PLAYER_H

#include "actor.h"

class Player: public Actor {
        public:
                Player();
                ~Player();
                bool is_player() { return true; }
        private:
                int health;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
