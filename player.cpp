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
        tiredness = 50;
        setchar('@');
        setname("Boris Hoffman");
}

Player::~Player()
{
        //dbg("Goodbye, cruel world.");
}

const char *Player::get_sanitydesc()
{
        if(sanity == 100)
                return sanitydesc[10];
        else if(sanity >= 91)
                return sanitydesc[9];
        else if(sanity >= 81)
                return sanitydesc[8];
        else if(sanity >= 71)
                return sanitydesc[7];
        else if(sanity >= 61)
                return sanitydesc[6];
        else if(sanity >= 41)
                return sanitydesc[5];
        else if(sanity >= 31)
                return sanitydesc[4];
        else if(sanity >= 21)
                return sanitydesc[3];
        else if(sanity >= 11)
                return sanitydesc[2];
        else if(sanity >= 3)
                return sanitydesc[1];
        else 
                return sanitydesc[0];
}

int Player::getstat(enum_stat stat)
{
        switch(stat) {
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
                case sTired:
                        return this->tiredness;
                        break;
                default:
                        return 0;
        }
}




// vim: fdm=syntax guifont=Terminus\ 8
