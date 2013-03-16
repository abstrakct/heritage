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
        init_commands(&this->cmd);

        name = "The Heritage of Efraim Edevane - Chapter I: [TBA]";
        version = "0.5.3";

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

void Game::end_turn()
{
        for(int i=0;i<12;i++) {
                if(npc[i].is_alive())
                        npc[i].ai();
        }

        player->endturn();
        player->look();
}

void Game::loop()
{
        command_type c;

        //console.print(10, 10, "Welcome to game!!");

        while (this->is_running()) {
                world->update_fov();
                display->update();

                c = cmd.get_command();
                switch(c) {
                        case cmd_exit:
                                endgame();
                                break;
                        case cmd_move_left:
                                player->move_left();
                                end_turn();
                                break;
                        case cmd_move_right:
                                player->move_right();
                                end_turn();
                                break;
                        case cmd_move_up:
                                player->move_up();
                                end_turn();
                                break;
                        case cmd_move_down:
                                player->move_down();
                                end_turn();
                                break;
                        case cmd_move_nw:
                                player->move_nw();
                                end_turn();
                                break;
                        case cmd_move_ne:
                                player->move_ne();
                                end_turn();
                                break;
                        case cmd_move_sw:
                                player->move_sw();
                                end_turn();
                                break;
                        case cmd_move_se:
                                player->move_se();
                                end_turn();
                                break;
                        case cmd_all_visible:
                                world->a->set_all_visible();
                                world->update_fov();
                                break;
                        case cmd_close_door:
                                world->close_nearest_door(player);
                                break;
                        case cmd_stairs:
                                player->use_stairs();
                                end_turn();
                                break;
                        case cmd_wait:
                                end_turn();
                                break;
                        default:
                                break;
                }
        }


        display->message("You have died...");
        display->update();
        display->get_key(true);
}

// vim: fdm=syntax guifont=Terminus\ 8
