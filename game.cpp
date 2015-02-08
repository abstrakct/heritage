/*
 * Roguelike Engine in C++
 *
 * The Game (loop etc.)
 *
 */

using namespace std;

#include <iostream>
#include <string>

#include "libtcod.hpp"
#include "actor.h"
#include "debug.h"
#include "display.h"
#include "game.h"
#include "player.h"
#include "npc.h"
#include "world.h"
#include "item.h"

extern Display *display;
extern Player *player;
extern NPC *npc;
extern World *world;
extern Game g;

Game::Game()
{
    init_commands(&this->cmd);

    name = "The Heritage of Efraim Edevane - Chapter I: [TBA]";
    version = "0.5.6";
    wizmode = false;
    clock.settime(20, 0, 0);

    running = true;
}

Game::~Game()
{
}

bool Game::is_running()
{
    return running;
}

void Game::endthegame()
{
    running = false;
}

void Game::intro()
{
}

void Game::end_turn()
{
    display->print_messages();
    display->touch();

    if(player->has_moved()) {
        for(int i=0;i<12;i++) {
            if(npc[i].is_alive()) {
                //if(player->can_see(npc[i].getx(), npc[i].gety()))
                //        display->message("You can see %s!", npc[i].getname());
                npc[i].ai();
            }
        }

        player->endturn();
        player->look();
    }
}

void Game::loop()
{
    command_type c;
    TCOD_key_t key;
    special_type type;
    SpecialMove sp;
    coord_t coord;

    //console.print(10, 10, "Welcome to game!!");

    while (this->is_running()) {
        world->update_fov();
        display->update();
        c = cmd.get_command();
        player->moved(false);
        switch(c) {
            case cmd_exit:
                endthegame();
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
            case cmd_close_door:
                world->close_nearest_door(player);
                break;
            case cmd_stairs:
                player->use_stairs();
                end_turn();
                break;
            case cmd_activate:
                player->area->cell[player->getx()][player->gety()].activate();
                break;
            case cmd_pickup:
                if(pcell.item) {
                    player->inv->add(pcell.item);
                    display->message("You now have %d items.", player->inv->num_items());
                    pcell.item = NULL;
                    player->moved();
                } else {
                    display->message("There is nothing here to pick up!");
                }
                display->touch();
                end_turn();
                break;
            case cmd_fight:
                coord = display->get_direction();
                if(player->area->cell[player->getx() + coord.x][player->gety() + coord.y].inhabitant) {
                    player->attack(player->area->cell[player->getx() + coord.x][player->gety() + coord.y].inhabitant);
                }
                display->touch();
                break;
            case cmd_wait:
                player->moved();
                end_turn();
                break;
            case cmd_special_1:
                type = player->get_special_type(1);
                if(type) {
                    sp = player->special.at(1);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_2:
                type = player->get_special_type(2);
                if(type) {
                    sp = player->special.at(2);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_3:
                type = player->get_special_type(3);
                if(type) {
                    sp = player->special.at(3);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_4:
                type = player->get_special_type(4);
                if(type) {
                    sp = player->special.at(4);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_5:
                type = player->get_special_type(5);
                if(type) {
                    sp = player->special.at(5);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_6:
                type = player->get_special_type(6);
                if(type) {
                    sp = player->special.at(6);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_7:
                type = player->get_special_type(7);
                if(type) {
                    sp = player->special.at(7);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_8:
                type = player->get_special_type(8);
                if(type) {
                    sp = player->special.at(8);
                    player->do_special(sp);
                }
                display->touch();
                break;
            case cmd_special_9:
                type = player->get_special_type(9);
                if(type) {
                    sp = player->special.at(9);
                    player->do_special(sp);
                }
                display->touch();
                break;
                // Debug/development commands:
            case cmd_all_visible:
                world->a->set_all_visible();
                world->update_fov();
                g.clock += 3700;
                wizmode = true;
                break;
            case cmd_incfear:
                player->incfear();
                end_turn();
                break;
            case cmd_give_powerfist:
                player->add_special(special_powerfist, true, body);
                end_turn();
                break;
            case cmd_give_mindblast:
                player->add_special(special_mindblast, true, mind);
                end_turn();
                break;
            default:
                break;
        }
    }

    display->messagec(COLOR_FATAL, "Game over. Press ESC to exit.");
    display->update();

    key = display->get_key(true);
    while(key.vk != TCODK_ESCAPE)
        key = display->get_key(true);
}

// vim: fdm=syntax guifont=Terminus\ 8
