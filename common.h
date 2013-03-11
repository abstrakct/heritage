/*
 * Common stuff
 *
 * Copyright (C) 2013
 *
 */

#ifndef _COMMON_H
#define _COMMON_H

#include "display.h"
#include "debug.h"

extern Display *display;

typedef struct {
        int x;
        int y;
} coord_t;

enum direction {
        none = 0,
        north,
        south,
        west,
        east,
        ne,
        nw,
        se,
        sw
};

bool fiftyfifty();

#endif

// vim: fdm=syntax guifont=Terminus\ 8
