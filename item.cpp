/*
 * Items
 */

#include <string>
using namespace std;

#include "item.h"


Item::Item(struct item_definition def)
{
    type = def.type;
    name = def.name;
    c = def.c;
    flags = def.flags;
}

Item::~Item()
{
}






// vim: fdm=syntax guifont=Terminus\ 8
