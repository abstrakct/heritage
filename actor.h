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

enum enum_stat {
        sMind = 0,
        sBody,
        sSoul,
        sSanity,
        sFear,
        sHealth
};

#define AI_RANDOM 1
#define AI_PATH 2

class Actor;

typedef void (Actor::*aifn)();

#include "world.h"

class Actor {
        public:
                Actor();
                virtual ~Actor() {  };
                virtual bool is_player() = 0;
                
                bool is_alive();
                bool is_male();
                void kill();
                void draw();
                void draw(TCODColor fg, TCODColor bg);
                coord_t getxy();
                void setxy(int x, int y);
                void setxy(coord_t newco);
                int  getx();
                int  gety();
                void setprevxy(int x, int y);
                void setprevxy(coord_t newco);
                void setchar(char newc);
                void setname(const char *name);
                void setcolors(TCODColor fg, TCODColor bg);
                void setgender(int i) { male = i; };
                char *getname();

                const char *get_sanitydesc();
                void decstat(enum_stat which, int amount = 1);
                void setstat(enum_stat which, int what);
                int  getstat(enum_stat which);

                void setfovradius(int amount) { fovradius = amount; };
                int  getfovradius() { return fovradius; };

                area_id_type area_id;

                // Movement
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
                bool alive;
                bool male;
                char c;
                char name[64];
                coord_t co;
                coord_t prev;
                e_role role;
                TCODColor fg, bg;
                int fovradius;

                // Stats
                int stat[6];
};

// Prototypes


#endif
// vim: fdm=syntax guifont=Terminus\ 8
