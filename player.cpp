/*
 * Roguelike Engine in C++
 *
 * Player specific stuff.
 *
 */

using namespace std;

#include <iostream>
#include <sstream>
#include <string>

#include "libtcod.hpp"
#include "game.h"
#include "actor.h"
#include "player.h"
#include "common.h"
#include "display.h"
#include "world.h"

extern World   *world;
extern Player  *player;
extern Display *display;
extern Game *game;

Player::Player()
{
        //body = mind = soul = 10;
        //sanity = 100;
        //fear = 0;
        //health = 100;

        /*setstat(sBody, 10);
        setstat(sMind, 10);
        setstat(sSoul, 10);*/
        setstat(sSanity, 100);
        setstat(sFear, 0);
        setstat(sHealth, 100);
        setchar('@');
        setcolors(TCODColor::white, TCODColor::red);
        setfovradius(17);
}

Player::~Player()
{
        //dbg("Goodbye, cruel world.");
}

void Player::look()
{
        cell_type type;

        type = world->get_cell_type(player->getxy());
        switch(type) {
                case stairs_up:
                        display->message("There is a staircase leading up here.");
                        break;
                case stairs_down:
                        display->message("There is a staircase leading down here.");
                        break;
                case cell_corpse:
                        display->message("You see here the corpse of %s. The sight fills you with horror.", world->a->cell[player->getx()][player->gety()].corpse->getname());
                        incfear();
                        break;
                case cell_bookcase:
                        display->message("You see a bookcase here, filled with all kinds of old-looking books.");
                        break;
                default:
                        break;
        }
        //if(world->a->cell[this->getx()][this->gety()].inhabitant != this)
        //        display->message("You are standing next to %s (%d,%d).", this->area->cell[this->getx()][this->gety()].inhabitant->getname(), this->getx(), this->gety());
}

void Player::die()
{
        display->message("You have died...");
        display->update();
        game->endgame();
}

void Player::create()
{
        char *name_c;
        string input;
        char c, d;
        int mind, body, soul;

        cout << "Welcome to " << game->name << " - v" << game->version << endl << endl;
        cout << "First, you will need to create your player character using this very simple character generator." <<endl;
        cout << "What's your name? ";
        getline(cin, input);
        name_c = new char[input.size()+1];
        name_c[input.size()] = 0;
        memcpy(name_c, input.c_str(), input.size());
        setname(name_c);
        cout << "Now we will randomly generate your stats:" << endl;
again:
        if(0) {            // do each one 3 times and take average result - makes it less likely to get stats in the upper or lower end of the scale. might be good for a "difficult mode"?
                mind = 0;
                mind += 10 + ri(-9, 10);
                mind += 10 + ri(-9, 10);
                mind += 10 + ri(-9, 10);
                mind /= 3;
                body = 0;
                body += 10 + ri(-9, 10);
                body += 10 + ri(-9, 10);
                body += 10 + ri(-9, 10);
                body /= 3;
                soul = 0;
                soul += 10 + ri(-9, 10);
                soul += 10 + ri(-9, 10);
                soul += 10 + ri(-9, 10);
                soul /= 3;
        } else {
                mind = 10 + ri(-9, 10);
                body = 10 + ri(-9, 10);
                soul = 10 + ri(-9, 10);
        }

        cout << "Mind (mental strength):    " << mind << endl;
        cout << "Body (physical strength):  " << body << endl;
        cout << "Soul (spiritual strength): " << soul << endl;
        cout << "Are you happy with these results (Y/n)? ";
        c = getchar();
        d = c;
        if(c == '\n')
                goto finish;
        c = getchar();
        if(d == 'n')
                goto again;
finish:
        setstat(sBody, body);
        setstat(sMind, mind);
        setstat(sSoul, soul);
}

void Player::use_stairs()
{
        if(player->area->cell[player->getx()][player->gety()].get_type() == stairs_up) {
                display->message("moving up!");
                world->current_area++;
                world->a = &world->area[world->current_area];
                player->area = world->a;
        } else if(player->area->cell[player->getx()][player->gety()].get_type() == stairs_down) {
                display->message("moving down!");
                world->current_area--;
                world->a = &world->area[world->current_area];
                player->area = world->a;
        } else {
                display->message("There are no stairs here!");
        }
}

void Player::endturn()
{
        decfear();
}



// vim: fdm=syntax guifont=Terminus\ 8
