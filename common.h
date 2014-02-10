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

#define d(x,y) dice(x,y,0)
#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

#ifdef DEBUG_ON
#define DBG(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define DBG(M, ...)
#endif

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

// Function prototypes
bool fiftyfifty();
int dice(int num, int sides, signed int modifier);
int ri(int a, int b);
signed int ability_modifier(int ab);

#endif

// vim: fdm=syntax guifont=Terminus\ 8
