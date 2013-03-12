/*
 * Roguelike Engine in C++
 *
 * Display handling with libtcod
 *
 */

using namespace std;

#include <iostream>
#include <vector>

#include "libtcod.hpp"
#include "debug.h"
#include "display.h"
#include "common.h"
#include "player.h"
#include "world.h"

extern World *world;
extern Player *player;

Display::Display()
{
        char t[] = "The Heritage of Efraim Eveningstar - Chapter I";

        set_resolution(1024, 600);
        set_title(t);

        TCODConsole::initRoot(chars_x, chars_y, title, false, TCOD_RENDERER_SDL);
        console = new TCODConsole(chars_x, chars_y);
        console->setCustomFont(FONT, TCOD_FONT_TYPE_GRAYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);
        console->root->setCustomFont(FONT, TCOD_FONT_TYPE_GRAYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 16, 16);
        console->root->setKeyboardRepeat(200, 50);

        map =     new TCODConsole(MAP_W, MAP_H);
        left =    new TCODConsole(LEFT_W, LEFT_H);
        bottom =  new TCODConsole(BOTTOM_W, BOTTOM_H);
}

Display::~Display()
{
        delete console;
}

void Display::set_resolution(int w, int h)
{
        width = w;
        height = h;
        chars_x = CHARS_X;
        chars_y = CHARS_Y;
        dbg("Set resolution to %d,%d - chars x,y = %d,%d", w, h, chars_x, chars_y);
}

void Display::set_title(char *window_title)
{
        strcpy(title, window_title);
}

TCOD_key_t Display::get_key(bool flush)
{
        console->root->flush();
        return console->root->checkForKeypress(TCOD_KEY_PRESSED);
}

char *Display::get_title()
{
        return title;
}

void Display::draw_game_screen()
{
        console->printFrame(LEFT_X, LEFT_Y, LEFT_W, LEFT_H, true);
        console->printFrame(MAP_X, MAP_Y, MAP_W, MAP_H, false);
        console->printFrame(BOTTOM_X, BOTTOM_Y, BOTTOM_W, BOTTOM_H, true);

}

void Display::update()
{
        this->draw_game_screen();
        world->draw_map();
        player->draw();

        //TCODConsole::blit(left, 0, 0, LEFT_W, LEFT_H, console, LEFT_X, LEFT_Y, 0.2, 0.2);
        //TCODConsole::blit(map, 0, 0, MAP_W, MAP_H, console, MAP_X, MAP_Y, 0.2, 0.2);
        //TCODConsole::blit(bottom, 0, 0, BOTTOM_W, BOTTOM_H, console, BOTTOM_X, BOTTOM_Y, 0.2, 0.2);

        // Handling printing messages here!

        display->message("TEST1");
        display->message(" TEST2");
        display->message("  TEST3");
        display->message("   TEST4");
        display->print_messages();
        
        TCODConsole::blit(console, 0, 0, chars_x, chars_y, TCODConsole::root, 0.1, 0.1);
        
        TCODConsole::flush();
}

void Display::put(int x, int y, int c, TCOD_bkgnd_flag_t flag)
{
        console->putChar(x, y, c, flag);
}

void Display::putmap(int x, int y, int c, TCOD_bkgnd_flag_t flag)
{
        console->putChar(MAP_X + x, MAP_Y + y, c, flag);
}

void Display::putmap(int x, int y, int c, TCODColor &fg, TCODColor &bg)
{
        console->putCharEx(MAP_X + x, MAP_Y + y, c, fg, bg);
}

void Display::print_messages()
{
        int y, j;
        vector <struct message_t>::iterator i; 

        y = 1;
        i = message_list.begin();
        for(j = 0; j <= message_list.size(); ++j) {
                if(y > 13)
                        break;
                console->print(BOTTOM_X + 1, BOTTOM_Y + y, i->message);
                ++y;
                ++i;
        }
}

void Display::message(const char *message)
{
        message_t item;

        item.num = 0;
        strcpy(item.message, message);

        message_list.push_back(item);

        //console->print(BOTTOM_X + 1, BOTTOM_Y + 13, message);
}



// vim: fdm=syntax guifont=Terminus\ 8
