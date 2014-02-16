/*
 * Items
 */


#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

#define IF_WIELDABLE  0x00000001
#define IF_POCKETABLE 0x00000002
#define IF_WEARABLE   0x00000004

enum item_type {
    it_nothing = 0,
    it_weapon,
    it_clothing,
    it_tool
};

/*
 * struct for how we define items 
 */
struct item_definition {
    string name;
    char c;
    item_type type;
    long flags;
};

class Item {
    private:
        string name;
        item_type type;
        long flags;
        char c;
    public:
        Item(struct item_definition def);
        ~Item();
};

#endif
