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
        sMind = 0,
        sBody,
        sSoul,
        sSanity,
        sFear,
        sHealth
};

class Player: public Actor {
        public:
                Player();
                ~Player();
                bool is_player() { return true; }
                int getstat(enum_stat which);
                const char *get_sanitydesc();
                void decstat(enum_stat which, int amount = 1);
                void setstat(enum_stat which, int what);
        private:
                // Stats
                int body;         // 1-20, how physically strong you are
                int mind;         // 1-20, how mentally strong you are
                int soul;         // 1-20, how spiritually strong you are (does that make sense?!)

                // "Resources"
                int sanity;       // 0-100, where 0 is completely bonkers and 100 is a mental health extravaganza
                int fear;         // 0-100, where 0 is zen master in nirvana, 100 is FUCKING SCARED MESSED MY PANTS
                int health;       // 0-100, where 0 is totally dead and gone, 100 is healthiest fucking person evar
                int stat[6];
};

#endif
// vim: fdm=syntax guifont=Terminus\ 8
