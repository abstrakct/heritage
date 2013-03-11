/*
 * Player specific stuff
 *
 * Copyright (C) 2013
 *
 */

#ifndef _PLAYER_H
#define _PLAYER_H

#include "actor.h"

#define PLAYER_STARTX (MAP_W / 2)
#define PLAYER_STARTY (MAP_H - 4)

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
