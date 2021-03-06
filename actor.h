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

enum special_move_t {
    body = 0,
    mind,
    soul,
    other
};

enum enum_stat {
    sMind = 0,
    sBody,
    sSoul,
    sSanity,
    sFear,
    sHealth
};

enum special_type {
    special_none = 0,
    special_mindblast,
    special_soulcrush,
    special_powerfist,
    special_zenmind,
    special_spiritsoul,
    special_toughenup,
    special_7,
    special_8,
    special_9,
};

extern const char *special_name[];

#define AI_RANDOM 1
#define AI_PATH 2

class Actor;

typedef void (Actor::*aifn)();

#include "world.h"

#define SPECIAL_ADD_FAIL     0
#define SPECIAL_ADD_SUCCESS  1
#define SPECIAL_ADD_INCREASE 2

class SpecialMove {
    public:
        SpecialMove();
        SpecialMove(special_type t);
        SpecialMove(special_type t, bool off);
        SpecialMove(special_type t, bool off, special_move_t a);
        ~SpecialMove() {
        };

        special_type type;
        special_move_t attack;
        char name[20];
        int level;
        bool offensive;
};

class Actor {
    public:
        Actor();
        virtual ~Actor() {  };
        virtual bool is_player() = 0;
        virtual void use_stairs();
        virtual void set_in_combat();

        bool is_alive() { return alive; };
        bool is_male();
        void kill();
        void draw();
        void draw(TCODColor fg, TCODColor bg);
        void drawcorpse();
        void setxy(int x, int y);
        void setxy(coord_t newco);
        int  getx();
        int  gety();
        const coord_t getxy() const;
        void setprevxy(int x, int y);
        void setprevxy(coord_t newco);
        void setchar(char newc);
        void setname(const char *name);
        void setcolors(TCODColor fg, TCODColor bg);
        void setgender(int i) { male = i; };
        const char *getname() const;

        const char *get_sanitydesc();
        void decstat(enum_stat which, int amount = 1);
        void incstat(enum_stat which, int amount = 1);
        void setstat(enum_stat which, int what);
        int  getstat(enum_stat which);
        void incfear();
        void decfear();
        void decsanity();

        void setfovradius(int amount) { fovradius = amount; };
        int  getfovradius() { return fovradius; };

        bool can_see(Actor *target);
        bool can_see(int x, int y);
        bool is_next_to(Actor *target);

        void attack(Actor *target, special_move_t type = body);
        void attack(Actor *target, SpecialMove sp);
        void attack_physical(Actor *target);
        void attack_physical(Actor *target, int d, int damage);
        void attack_mind(Actor *target, int d, int damage);

        void attack_powerfist(Actor *target, SpecialMove sp);
        void attack_mindblast(Actor *target, SpecialMove sp);
        //void attack_soulcrush(Actor *target, SpecialMove sp);

        bool pass_roll(enum_stat stat);
        bool pass_roll(enum_stat stat, int i);
        bool has_special();
        int  add_special(special_type t); 
        int  add_special(special_type t, bool off);
        int  add_special(special_type t, bool off, special_move_t a);
        special_type get_special_type(int i);
        int get_special_level(int i);
        void do_special(SpecialMove sp);

        // Movement
        void move(int dx, int dy);
        void move_left();
        void move_right();
        void move_up();
        void move_down();
        void move_nw();
        void move_ne();
        void move_sw();
        void move_se();
        void moved(bool b = true) { moved_ = b; };
        bool has_moved() { return moved_; };

        area_id_type area_id;
        Actor *enemy;              // public enemy haha!
        Area *area;                // in which area is this actor?
        bool alive;
        vector <SpecialMove> special;
        Inventory *inv;
    protected:
    private:
        bool male;
        bool moved_;
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
