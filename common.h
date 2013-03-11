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
#include "world.h"

extern Display *display;
extern World *world;
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

#endif

// vim: fdm=syntax guifont=Terminus\ 8
