/*
 * Debugging
 *
 * Copyright (C) 2013
 *
 */

#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>

// comment out following line if you don't want debug mode!
#define DEBUG_MODE

// the following was stolen from http://stackoverflow.com/questions/4248372/custom-debug-printf-used-at-runtime
#ifdef DEBUG_MODE
#define dbg(fmt, ...) printf(("  DEBUG: [%s:%d] %s: " fmt "\n"), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);
#else
#define dbg(...)
#endif

#endif

// vim: fdm=syntax guifont=Terminus\ 8
