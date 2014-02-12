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

class Game {
        public:
                Game();
                ~Game();
                void loop();
                bool is_running();
                void endgame();
                void intro();
                void end_turn();
                
                const char *name;  // the name of the game!
                const char *version;
                bool wizmode;
                Clock clock;
        protected:
        private:
                Command cmd;
                bool running;
};

#endif

// vim: fdm=syntax guifont=Terminus\ 8
