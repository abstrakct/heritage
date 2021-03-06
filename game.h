/*
 * The Game
 *
 * Copyright (C) 2013
 *
 */

#ifndef _GAME_H
#define _GAME_H

#include "common.h"
#include "command.h"
#include "display.h"
#include "item.h"

class Game {
    public:
        Game();
        ~Game();
        void loop();
        bool is_running();
        void endthegame();
        void intro();
        void end_turn();

        const char *name;  // the name of the game!
        const char *version;
        bool wizmode;
        Clock clock;
        vector<Item> itemdef;
        Command cmd;
    protected:
    private:
        bool running;
};

#endif

// vim: fdm=syntax guifont=Terminus\ 8
