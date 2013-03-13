/*
 * Roguelike Engine in C++
 *
 * Player specific stuff.
 *
 */

using namespace std;

#include <iostream>

#include "actor.h"
#include "player.h"
#include "common.h"

const char *sanitydesc[] = {
        "Member of WBC",              //  0 - 3
        "Completely insane",          //  3 - 10
        "Raving and drooling",        // 11 - 20
        "Nervous breakdown imminent", // 21 - 30
        "You're losing it",           // 31 - 40
        "Average man in the street",  // 41 - 60
        "Less insane than most",      // 61 - 70
        "Still doing great",          // 71 - 80
        "Pretty darn sane",           // 81 - 90 
        "Completely sane",            // 91 - 99
        "Mental health poster child", // 100
};


Player::Player()
{
        body = mind = soul = 10;
        sanity = 100;
        fear = 0;
        health = 100;
        setstat(sBody, 10);
        setstat(sMind, 10);
        setstat(sSoul, 10);
        setstat(sSanity, 100);
        setstat(sFear, 0);
        setstat(sHealth, 100);
        setchar('@');
        setname("Boris Hoffman");
}

Player::~Player()
{
        //dbg("Goodbye, cruel world.");
}

const char *Player::get_sanitydesc()
{
        if(getstat(sSanity) == 100)
                return sanitydesc[10];
        else if(getstat(sSanity) >= 91)
                return sanitydesc[9];
        else if(getstat(sSanity) >= 81)
                return sanitydesc[8];
        else if(getstat(sSanity) >= 71)
                return sanitydesc[7];
        else if(getstat(sSanity) >= 61)
                return sanitydesc[6];
        else if(getstat(sSanity) >= 41)
                return sanitydesc[5];
        else if(getstat(sSanity) >= 31)
                return sanitydesc[4];
        else if(getstat(sSanity) >= 21)
                return sanitydesc[3];
        else if(getstat(sSanity) >= 11)
                return sanitydesc[2];
        else if(getstat(sSanity) >= 3)
                return sanitydesc[1];
        else 
                return sanitydesc[0];
}

int Player::getstat(enum_stat which)
{
        return this->stat[(int)which];

        /*switch(which) {
                case sMind:
                        return this->mind;
                        break;
                case sBody:
                        return this->body;
                        break;
                case sSoul:
                        return this->soul;
                        break;
                case sSanity:
                        return this->sanity;
                        break;
                case sFear:
                        return this->fear;
                        break;
                case sHealth:
                        return this->health;
                        break;
                default:
                        return 0;
        }*/
}

void Player::setstat(enum_stat which, int what)
{
        this->stat[which] = what;
}

void Player::decstat(enum_stat which, int amount)
{
        this->stat[which] -= amount;
}


// vim: fdm=syntax guifont=Terminus\ 8
