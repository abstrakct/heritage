/*
 * Roguelike Engine in C++
 *
 * Display handling with libtcod
 *
 */

using namespace std;

#include <iostream>

#include "libtcod.hpp"
#include "debug.h"
#include "display.h"

Display::Display()
{
        char t[] = "Test";

        set_resolution(1024, 600);
        set_title(t);

        console = new TCODConsole(chars_x, chars_y);
        console->setCustomFont(FONT, TCOD_FONT_TYPE_GRAYSCALE | TCOD_FONT_LAYOUT_ASCII_INROW, 0, 0);
        console->initRoot(chars_x, chars_y, title, false, TCOD_RENDERER_SDL);
        console->setKeyboardRepeat(350, 70);

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
        console->flush();
        return console->checkForKeypress(TCOD_KEY_PRESSED);
}

char *Display::get_title()
{
        return title;
}

void Display::draw_game_screen()
{
        left->printFrame(LEFT_X, LEFT_Y, LEFT_W, LEFT_H, true);
        map->printFrame(0, 0, MAP_W, MAP_H, false);
        bottom->printFrame(0, 0, BOTTOM_W, BOTTOM_H, true);

}

void Display::update()
{
        this->draw_game_screen();

        TCODConsole::blit(left, 0, 0, LEFT_W, LEFT_H, console, LEFT_X, LEFT_Y, 0.2, 0.2);
        TCODConsole::blit(map, 0, 0, MAP_W, MAP_H, console, MAP_X, MAP_Y, 0.2, 0.2);
        TCODConsole::blit(bottom, 0, 0, BOTTOM_W, BOTTOM_H, console, BOTTOM_X, BOTTOM_Y, 0.2, 0.2);
        TCODConsole::blit(console, 0, 0, chars_x, chars_y, TCODConsole::root, 0.1, 0.1);
        
        TCODConsole::flush();
}

void Display::put(int x, int y, int c, TCOD_bkgnd_flag_t flag)
{
        console->putChar(x, y, c, flag);
}

void Display::putmap(int x, int y, int c, TCOD_bkgnd_flag_t flag)
{
        map->putChar(x, y, c, flag);
}
// vim: fdm=syntax guifont=Terminus\ 8
