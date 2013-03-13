/*
 * Roguelike Engine in C++
 *
 * The Game (loop etc.)
 *
 */

using namespace std;

#include <iostream>
#include <vector>

#include "libtcod.hpp"
#include "command.h"
#include "common.h"
#include "debug.h"

command_t command_set_normal[] = {
        { { TCODK_CHAR,   0,  1, 0, 0, 0, 0, 0 }, cmd_nothing },
        { { TCODK_ESCAPE, 0,  1, 0, 0, 0, 0, 0 }, cmd_exit },
//      { { TCODK_CHAR,  'q', 1, 0, 0, 0, 0, 0 }, cmd_exit },
        { { TCODK_CHAR,  'j', 1, 0, 0, 0, 0, 0 }, cmd_move_down },
        { { TCODK_DOWN,   0,  1, 0, 0, 0, 0, 0 }, cmd_move_down },
        { { TCODK_KP2,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_down },
        { { TCODK_CHAR,  'k', 1, 0, 0, 0, 0, 0 }, cmd_move_up },
        { { TCODK_UP,     0,  1, 0, 0, 0, 0, 0 }, cmd_move_up },
        { { TCODK_KP8,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_up },
        { { TCODK_CHAR,  'h', 1, 0, 0, 0, 0, 0 }, cmd_move_left },
        { { TCODK_LEFT,   0,  1, 0, 0, 0, 0, 0 }, cmd_move_left },
        { { TCODK_KP4,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_left },
        { { TCODK_CHAR,  'l', 1, 0, 0, 0, 0, 0 }, cmd_move_right },
        { { TCODK_RIGHT,  0,  1, 0, 0, 0, 0, 0 }, cmd_move_right },
        { { TCODK_KP6,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_right },
        { { TCODK_CHAR,  'y', 1, 0, 0, 0, 0, 0 }, cmd_move_nw },
        { { TCODK_KP7,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_nw },
        { { TCODK_CHAR,  'u', 1, 0, 0, 0, 0, 0 }, cmd_move_ne },
        { { TCODK_KP9,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_ne },
        { { TCODK_CHAR,  'b', 1, 0, 0, 0, 0, 0 }, cmd_move_sw },
        { { TCODK_KP1,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_sw },
        { { TCODK_CHAR,  'n', 1, 0, 0, 0, 0, 0 }, cmd_move_se },
        { { TCODK_KP3,    0,  1, 0, 0, 0, 0, 0 }, cmd_move_se },
        { { TCODK_KP5,    0,  1, 0, 0, 0, 0, 0 }, cmd_wait },
        { { TCODK_CHAR,  '.', 1, 0, 0, 0, 0, 0 }, cmd_wait },
        { { TCODK_CHAR,  'c', 1, 0, 0, 0, 0, 0 }, cmd_close_door },

        // debug / development commands
        { { TCODK_F1,     0,  1, 0, 0, 0, 0, 0 }, cmd_all_visible },
};

Command::Command()
{
}

Command::~Command()
{
}

void Command::add_command(TCOD_key_t key, command_type cmd)
{
        command_t c;

        c.cmd = cmd;
        c.key = key;

        command_list.push_back(c);
        //dbg("added command %d char %c command_type %d", c.cmd, key.c, cmd);
}

command_type Command::get_command()
{
        vector<struct command_t>::iterator i;

        TCOD_key_t key = display->get_key(true);
        //dbg("Got key: vk = %d   c = %c (%d)", key.vk, key.c, key.c);

        for(i = command_list.begin(); i != command_list.end(); ++i) {
                if(key.vk == TCODK_CHAR) {
                        if(key.c == i->key.c) {
                                return i->cmd;
                        }
                } else if(key.vk == i->key.vk) {
                        return i->cmd;
                }
                
                //if(key.vk == 65) {
                //        ret = cmd_nothing;
                //}
        }

        return cmd_nothing;
}

void init_commands(Command *c)
{
        int num, i;

        num = sizeof(command_set_normal) / sizeof(struct command_t);
        for(i = 0; i < num; i++)
                c->add_command(command_set_normal[i].key, command_set_normal[i].cmd);

}
// vim: fdm=syntax guifont=Terminus\ 8
