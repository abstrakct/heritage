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
extern Game g;

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
        setfovradius(33);
}

Player::~Player()
{
        //dbg("Goodbye, cruel world.");
}

void Player::look()
{
        cell_type type;

        type = world->get_cell_type(player->area, player->getxy());
        switch(type) {
                case stairs_up:
                        display->messagec(COLOR_INFO, "There is a staircase leading up here.");
                        break;
                case stairs_down:
                        display->messagec(COLOR_INFO, "There is a staircase leading down here.");
                        break;
                case cell_corpse:
                        display->messagec(COLOR_HORROR, "You see here the corpse of %s. The sight fills you with horror.", world->a->cell[player->getx()][player->gety()].corpse->getname());
                        incfear();
                        break;
                case cell_bookcase:
                        display->messagec(COLOR_INFO, "You see a bookcase here, filled with all kinds of old-looking books.");
                        break;
                case cell_chair:
                        display->messagec(COLOR_INFO, "There is a chair here.");
                        break;
                case cell_table:
                        display->messagec(COLOR_INFO, "There is a table here.");
                        break;
                case cell_coffin:
                        display->messagec(COLOR_INFO, "There is a coffin here! How macabre!");
                        break;
                case cell_candle:
                        display->messagec(COLOR_INFO, "There is a large candle holder here, with lit candles in it.");
                        break;
                default:
                        break;
        }
        if(world->a->cell[this->getx()][this->gety()].item)
            display->messagec(COLOR_GOOD, "There is a %s here.", world->a->cell[this->getx()][this->gety()].item->get_name());
        //if(world->a->cell[this->getx()][this->gety()].inhabitant != this)
        //        display->message("You are standing next to %s (%d,%d).", this->area->cell[this->getx()][this->gety()].inhabitant->getname(), this->getx(), this->gety());
}

void Player::die()
{
        display->messagec(COLOR_FATAL, "You have died...");
        display->update();
        g.endthegame();
}

void Player::create()
{
        char *name_c;
        string input;
        char c, d;
        int mind, body, soul;

        cout << "Welcome to " << g.name << " - v" << g.version << endl << endl;
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
                world->clear_inhabitant(player->area, player->getxy());
                world->current_area++;
                world->a = &world->area[world->current_area];
                player->area = world->a;
                player->setxy(player->area->stairs_down);
                world->set_inhabitant(player);
                display->clear();
                display->update();
        } else if(player->area->cell[player->getx()][player->gety()].get_type() == stairs_down) {
                world->clear_inhabitant(player->area, player->getxy());
                world->current_area--;
                world->a = &world->area[world->current_area];
                player->area = world->a;
                player->setxy(player->area->stairs_up);
                world->set_inhabitant(player);
                display->clear();
                display->update();
        } else {
                display->messagec(COLOR_ERROR, "There are no stairs here!");
        }
}

void Player::endturn()
{
        decfear();
        g.clock += ri(5,10);
}



// vim: fdm=syntax guifont=Terminus\ 8
