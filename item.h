/*
 * The Items
 *
 * Copyright (C) rkaid <rkaidstudios@gmail.com> 2013, 2014
 * 
 * See LICENSE.txt for licensing information.
 *
 */


#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "common.h"

#define IF_WIELDABLE  0x00000001
#define IF_CAN_CUT    0x00000002
#define IF_WEARABLE   0x00000004

enum item_type {
    it_nothing = 0,
    it_weapon,
    it_clothing,
    it_tool,
    it_key,
};

/*
 * struct for how we define items 
 */
struct item_definition {
    string name;
    char c;
    item_type type;
    long flags;
    int attack;
    int defense;
    int chance;
};

class Item {
    private:
        string name;
        item_type type;
        long flags;
        int attack;
        int defense;
    public:
        Item(struct item_definition def);
        Item(Item *item);
        Item();
        ~Item();
        //string get_name() { return name; };
        char *get_name();

        coord_t co;
        int chance;      // spawn chance
        char c;
        char key;
        TCODColor fg, bg;
};

class Inventory {
    public:
        Inventory();
        ~Inventory();
        
        int num_items();
        void add(Item *i);
        Item get(int x);

    private:
        vector<Item> items;
        char keys[13]; 
};

#endif



// vim: fdm=syntax guifont=Terminus\ 8
