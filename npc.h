/*
 * NPC specific stuff
 *
 * Copyright (C) 2013
 *
 */

#ifndef _NPC_H
#define _NPC_H

#include "actor.h"

class NPC: public Actor {
        public:
                NPC();
                ~NPC();
                bool is_player() { return false; }
        private:
                int health;
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8

