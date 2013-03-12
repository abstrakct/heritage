/*
 * Roguelike Engine in C++
 *
 * The Game (loop etc.)
 *
 */

using namespace std;

#include <iostream>

#include "libtcod.hpp"
#include "actor.h"
#include "debug.h"
#include "display.h"
#include "game.h"
#include "player.h"
#include "npc.h"
#include "world.h"

extern Display *display;
extern Player *player;
extern NPC *npc;
extern World *world;

Game::Game()
{
        display = new Display;

        init_commands(&this->cmd);

        running = true;
}

Game::~Game()
{
}

bool Game::is_running()
{
        return running;
}

void Game::endgame()
{
        running = false;
}

void Game::intro()
{
}

void Game::loop()
{
        command_type c;

        //console.print(10, 10, "Welcome to game!!");

        while (this->is_running()) {
                //dbg("Player coords = %d, %d", player->getx(), player->gety());

                world->update_fov();
                player->draw();
                display->update();

                c = cmd.get_command();
                switch(c) {
                        case cmd_exit:
                                endgame();
                                break;
                        case cmd_move_left:
                                player->move_left();
                                break;
                        case cmd_move_right:
                                player->move_right();
                                break;
                        case cmd_move_up:
                                player->move_up();
                                break;
                        case cmd_move_down:
                                player->move_down();
                                break;
                        case cmd_move_nw:
                                player->move_nw();
                                break;
                        case cmd_move_ne:
                                player->move_ne();
                                break;
                        case cmd_move_sw:
                                player->move_sw();
                                break;
                        case cmd_move_se:
                                player->move_se();
                                break;
                        case cmd_all_visible:
                                world->a->set_all_visible();
                                world->update_fov();
                                break;
                        default:
                                break;
                }
        }
}

// vim: fdm=syntax guifont=Terminus\ 8
