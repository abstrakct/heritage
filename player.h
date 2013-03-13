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

enum enum_stat {
        sMind = 1,
        sBody,
        sSoul,
        sSanity,
        sFear,
        sTired
};

class Player: public Actor {
        public:
                Player();
                ~Player();
                bool is_player() { return true; }
                int getstat(enum_stat stat);
                const char *get_sanitydesc();
        private:
                // Stats
                int body;         // 1-20, how physically strong you are
                int mind;         // 1-20, how mentally strong you are
                int soul;         // 1-20, how spiritually strong you are (does that make sense?!)

                // "Resources"
                int sanity;       // 0-100, where 0 is completely bonkers and 100 is a mental health extravaganza
                int fear;         // 0-100, where 0 is zen master in nirvana, 100 is FUCKING SCARED MESSED MY PANTS
                int tiredness;    // 0-100, where 0 is perfectly well-rested, 100 is I'd actually literally rather die than try to move one more inch
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
