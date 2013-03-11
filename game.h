/*
 * The Game
 *
 * Copyright (C) 2013
 *
 */

#ifndef _GAME_H
#define _GAME_H

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
        protected:
        private:
                Command cmd;
                bool running;
};

#endif

// vim: fdm=syntax guifont=Terminus\ 8
