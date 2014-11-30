/*
 * Items
 *
 * Copyright (C) rkaid <rkaidstudios@gmail.com> 2013, 2014
 * 
 * See LICENSE.txt for licensing information.
 *
 */

using namespace std;
#include <iostream>
#include <string>

#include <unistd.h>
#include "item.h"


Item::Item(struct item_definition def)
{
    type = def.type;
    name = def.name;
    c = def.c;
    flags = def.flags;
    attack = def.attack;
    defense = def.defense;
    chance = def.chance;
    co.x = co.y = 0;
    fg = TCODColor::black;
    bg = TCODColor::black;
    key = 0;
}

Item::Item()
{
}

Item::Item(Item *item)
{
    *this = *item;
}

Item::~Item()
{
}

char *Item::get_name()
{
    size_t i;
    char *ret = new char[200];

    for(i=0;i<=this->name.size();++i)
        ret[i] = this->name[i];

    return ret;
}

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

int Inventory::num_items()
{
    int ret = 0;

    for(unsigned int i = 0; i < items.size(); i++)
        ret++;

    return ret;
}

Item Inventory::get(int x)
{
    Item ret;
    vector<Item>::iterator it;
    int i;

    it = items.begin();
    for(i = 0; i != x; i++)
        it++;

    ret = *it;
    return ret;
}

void Inventory::add(Item *i)
{
    i->key = this->num_items() + 'a';
    this->items.push_back(*i);
}

// vim: fdm=syntax guifont=Terminus\ 8
