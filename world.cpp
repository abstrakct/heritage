/*
 * Roguelike Engine in C++
 *
 * Everything related to the world itself - maps, levels, generators, whatnot.
 *
 */

using namespace std;

#include <iostream>

#include "actor.h"
#include "world.h"
#include "common.h"
#include "player.h"
#include "game.h"

extern Player *player;
extern World *world;
extern Game g;

const char *area_name[] = {
    "Cellar, level 6",
    "Cellar, level 5",
    "Cellar, level 4",
    "Cellar, level 3",
    "Cellar, level 2",
    "Cellar, level 1",
    "1st floor",
    "2nd floor",
    "3rd floor",
    "4th floor",
    "5th floor",
    "6th floor",
};

class MyCallback : public ITCODBspCallback
{
    public :
        bool visitNode(TCODBsp *node, void *userData)
        {
            //dbg("making room, x=%d y=%d w=%d h=%d", node->x, node->y, node->w, node->h);
            world->a->make_room_with_doors(node->x, node->y, node->x + node->w-2, node->y + node->h-2);

            /*if(node->horizontal) {
                int x1 = node->x - 2;
                int y1 = node->y - 2;
                int x2 = node->x + node->w - 2;
                int y2 = node->y + node->h - 2;
                world->a->make_door(x1, ri(y1, y2), false);
                world->a->make_door(x2, ri(y1, y2), false);
            } else {
                int x1 = node->x;
                int y1 = node->y;
                int x2 = node->x + node->w - 2;
                int y2 = node->y + node->h - 2;
                world->a->make_door(ri(x1, x2), y1, false);
                world->a->make_door(ri(x1, x2), y2, false);
            }*/

            return true;
        }
};

/*
 * class: Cell
 */
Cell::Cell()
{
    flags = 0;
    visible = false;
    activated = false;
    cell_seen = false;
    inhabitant = NULL;
    corpse = NULL;
    c = 0;
    item = 0;

    this->set_floor();
}

Cell::~Cell()
{
}

bool Cell::is_walkable()
{
    if(this->inhabitant) {
        return false;
    }

    switch(this->type) {
        case floor:
        case door_open:
        case door_closed:
        case stairs_up:
        case stairs_down:
        case cell_corpse:
        case cell_bookcase:
        case cell_table:
        case cell_chair:
        case cell_coffin:
        case cell_candle:
            return true;
        case wall:
        case nothing:
            return false;
        default:
            return false;
    }

    return false;
}

bool Cell::is_transparent()
{
    switch(this->type) {
        case floor:
        case door_open:
        case stairs_up:
        case stairs_down:
        case cell_corpse:
        case cell_bookcase:
        case cell_table:
        case cell_chair:
        case cell_coffin:
        case cell_candle:
            return true;
        case wall:
        case door_closed:
        case nothing:
            return false;
        default:
            return true;
    }
}

bool Cell::is_visible()
{
    return this->visible; 
}

void Cell::set_wall()
{
    this->type = wall;
    this->fg = TCODColor::darkerFlame;
    this->bg = TCODColor::black;
    c = '#';
}

void Cell::set_floor()
{
    this->type = floor;
    this->fg = TCODColor::darkerGrey;
    this->bg = TCODColor::black;
    c = '.';
}

void Cell::set_stairs_up()
{
    this->type = stairs_up;
    this->fg = TCODColor::yellow;
    this->bg = TCODColor::black;
    c = '<';
}

void Cell::set_stairs_down()
{
    this->type = stairs_down;
    this->fg = TCODColor::yellow;
    this->bg = TCODColor::black;
    c = '>';
}

void Cell::set_door_closed()
{
    this->type = door_closed;
    this->fg = TCODColor::grey;
    this->bg = TCODColor::black;
    c = '+';
}

void Cell::set_door_open()
{
    this->type = door_open;
    this->fg = TCODColor::grey;
    this->bg = TCODColor::black;
    c = '\\';
}

void Cell::set_corpse(Actor *who)
{
    this->type = cell_corpse;
    this->fg = TCODColor::black;
    this->bg = TCODColor::red;
    c = 0x0F;
    corpse = who;
}

void Cell::set_bookcase()
{
    this->type = cell_bookcase;
    this->fg = TCODColor::lightestAzure;
    this->bg = TCODColor::black;
    c = 'H';
}

void Cell::set_chair()
{
    this->type = cell_chair;
    this->fg = TCODColor::azure;
    this->bg = TCODColor::black;
    c = 'h';
}

void Cell::set_table()
{
    this->type = cell_table;
    this->fg = TCODColor::lightFlame;
    this->bg = TCODColor::black;
    c = 'T';
}

void Cell::set_coffin()
{
    this->type = cell_coffin;
    this->fg = TCODColor::darkestFlame;
    this->bg = TCODColor::black;
    c = 0x09;
}

void Cell::set_candle()
{
    this->type = cell_candle;
    this->fg = TCODColor::flame;
    this->bg = TCODColor::black;
    c = 0x05;
}

void Cell::set_visibility(bool b)
{
    this->visible = b;
}

void Cell::draw(int x, int y)
{
    if(inhabitant) {
        if(inhabitant->alive) {
            if(inhabitant->area == world->a) {
                inhabitant->draw();
            }
        }
    } else {
        display->putmap(x, y, this->c, this->fg, this->bg);
    }
    display->touch();
}

void Cell::draw(int x, int y, TCODColor fore, TCODColor back)
{
    if(inhabitant) {
        if(inhabitant->alive) {
            if(inhabitant->area == world->a) {
                inhabitant->draw(fore, back);
            }
        }
    } else {
        display->putmap(x, y, this->c, fore, back);
    }
    display->touch();
}

void Cell::set_color(TCODColor fg, TCODColor bg)
{
    this->fg = fg;
    this->bg = bg;
}

cell_type Cell::get_type()
{
    return this->type;
}

void Cell::activate_bookcase()
{
    int i, di, book, result;

    if(activated) {
        display->messagec(COLOR_INFO, "You find nothing more of interest in this bookcase.");
        return;
    }

    display->message(" ");
    display->messagec(COLOR_BOOK, "You spend a little while browsing the books in this bookcase, looking for something interesting.");

    i = ri(1, 20);
    di = ri(10, 15);
    if(i <= di) {
        book = ri(1, 7);
        switch(book) {
            case 1:
                display->messagec(COLOR_BOOK, "An old book titled \"Unlocking Your Brain's True Potential - Vol. I\" catches your eye.");
                display->messagec(COLOR_BOOK, "The book is written by Dr. Evan Hoffman.");
                if(player->pass_roll(sMind)) {
                    if(fiftyfifty()) {
                        int a = ri(1,5);
                        switch(a) {
                            case 1:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XIII - Telekinetics and related subjects\" and read for a while.");
                                break;
                            case 2:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XII - Mindblasting - Fact or Fiction?\" and read for a while.");
                                break;
                            case 3:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XV - Improving the Flow\" and read for a while.");
                                break;
                            case 4:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter IX - S.L.: A Case Study\" and read for a while.");
                                break;
                            case 5:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XXI - Beyond the Mind\" and read for a while.");
                                break;
                        }
                        result = player->add_special_attack(special_mindblast);
                        if(result == SPECIAL_ADD_SUCCESS)
                            display->messagec(COLOR_GOOD, "Congratulations! You can now employ your mind to cause minor destruction!");
                        if(result == SPECIAL_ADD_INCREASE)
                            display->messagec(COLOR_GOOD, "Congratulations! You increase the power of your mindblast!");
                    } else {
                        int a = ri(1,5);
                        switch(a) {
                            case 1:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XVI - Defensive techniques for a healthy mind\" and read for a while.");
                                break;
                            case 2:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XVII - Buddhism for beginners\" and read for a while.");
                                break;
                            case 3:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XVIII - Meditation: Does it work?\" and read for a while.");
                                break;
                            case 4:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XIX - Zen against Anxiety\" and read for a while.");
                                break;
                            case 5:
                                display->messagec(COLOR_BOOK, "You open the book at \"Chapter XX - Finding Your Personal Nirvana\" and read for a while.");
                                break;
                        }
                        result = player->add_special_attack(special_zenmind);
                        if(result == SPECIAL_ADD_SUCCESS)
                            display->messagec(COLOR_GOOD, "Congratulations! You can now calm your fears using your own mind!");
                        if(result == SPECIAL_ADD_INCREASE)
                            display->messagec(COLOR_GOOD, "Congratulations! You increase the power of your zenmind!");
                    }
                } else {
                    display->messagec(COLOR_BOOK, "You flip randomly through the book, but the medical terminology goes straight over your head. You put the book back on the shelf.");
                }
                display->message(" ");
                break;
            case 2:
                display->messagec(COLOR_BOOK, "You come across a large book called \"How To Fight - A Reference\". It looks interesting.");
                if(player->pass_roll(sMind)) {
                    if(fiftyfifty()) {
                        display->messagec(COLOR_BOOK, "You read through the book, picking up a few useful fighting techniques.");
                        result = player->add_special_attack(special_powerfist);
                        if(result == SPECIAL_ADD_SUCCESS)
                            display->messagec(COLOR_GOOD, "Congratulations! You can now use a special move in physical combat!");
                        if(result == SPECIAL_ADD_INCREASE)
                            display->messagec(COLOR_GOOD, "Congratulations! You learn a few more techniques for physical combat!");
                    } else {
                        display->messagec(COLOR_BOOK, "You read through the book, picking up a few useful defensive techniques. You feel tougher already!");
                        result = player->add_special_attack(special_toughenup);
                        if(result == SPECIAL_ADD_SUCCESS)
                            display->messagec(COLOR_GOOD, "Congratulations! You can now defend yourself more easily, and feel less anxious overall!");
                        if(result == SPECIAL_ADD_INCREASE)
                            display->messagec(COLOR_GOOD, "Congratulations! You toughen up even more!");
                    }
                } else {
                    display->messagec(COLOR_BOOK, "You flip through the book, but see nothing you don't already know.");
                }
                display->message(" ");
                break;
            case 3:
                display->messagec(COLOR_BOOK, "You find a small book called \"Mysteries of the Soul\", written by Marvin E. A. Edeef.");
                if(player->pass_roll(sMind)) {
                    if(fiftyfifty()) {
                        display->messagec(COLOR_BOOK, "As you flip through the book you come to a sudden realization about the soul and your own spirituality.");
                        result = player->add_special_attack(special_soulcrush);
                        if(result == SPECIAL_ADD_SUCCESS)
                            display->messagec(COLOR_GOOD, "Congratulations! You can now use some of the powers of your soul to help you make it through the night!");
                        if(result == SPECIAL_ADD_INCREASE)
                            display->messagec(COLOR_GOOD, "Congratulations! You increase the power of your soulcrushing ability!");
                    } else {
                        display->messagec(COLOR_BOOK, "As you flip through the book you come to a sudden realization about the soul and your own spirituality.");
                        result = player->add_special_attack(special_spiritsoul);
                        if(result == SPECIAL_ADD_SUCCESS)
                            display->messagec(COLOR_GOOD, "Congratulations! Your soul is now connected more closely to the spiritual world. You feel safer!");
                        if(result == SPECIAL_ADD_INCREASE)
                            display->messagec(COLOR_GOOD, "Congratulations! You increase the power of your spiritsoul!");
                    }
                } else {
                    display->messagec(COLOR_BOOK, "You flip through the book, quickly concluding that it's just New-Age mumbo jumbo.");
                }
                display->message(" ");
                break;
            case 4:
                display->messagec(COLOR_BOOK, "Your gaze stops at a book titled \"Physical Excercises for Modern Men and Women\".");
                if(player->pass_roll(sMind)) {
                    display->messagec(COLOR_BOOK, "Inside the book are pictures and descriptions of various physical excercises. You take notice of some of these.");
                    int amount;
                    if(fiftyfifty())
                        amount = dice(1,3,0);
                    else
                        amount = 1;
                    player->incstat(sBody, amount);
                    display->messagec(COLOR_GOOD, "Congratulations! You feel more prepared for physical combat. (Body +%d)", amount);
                } else {
                    display->messagec(COLOR_BOOK, "Inside the book are pictures and descriptions of various physical excercises.");
                    display->messagec(COLOR_BOOK, "You discard the book. Books are no substitute for real physical excercise!");
                }
                display->message(" ");
                break;
            case 5:
                display->messagec(COLOR_BOOK, "You discover an issue of a magazine entitled \"Chess Puzzles Monthly\".");
                if(player->pass_roll(sMind)) {
                    display->messagec(COLOR_BOOK, "You spend some time solving a few of the puzzles inside - even some of the hard ones!");
                    int amount;
                    if(fiftyfifty())
                        amount = dice(1,3,0);
                    else
                        amount = 1;

                    player->incstat(sMind, amount);
                    display->messagec(COLOR_GOOD, "Congratulations! You can literally feel your brain expanding. (Mind +%d)", amount);
                } else {
                    display->messagec(COLOR_BOOK, "You try to solve some of the puzzles, but fail miserably at each and every one.");
                    display->messagec(COLOR_BOOK, "Frustrated, you tear the magazine apart and throw it away.");
                }
                display->message(" ");
                break;
            case 6:
                display->messagec(COLOR_BOOK, "Amongst a collection of pulp novels, you come across a thick tome called \"Religions and Rituals - Then and Now\".");
                display->messagec(COLOR_BOOK, "The author's name is Ann Theodorette Ropos.");
                if(player->pass_roll(sMind)) {
                    display->messagec(COLOR_BOOK, "The tome is quite academic in style, but you manage to understand some of the main principles and ideas.");
                    int amount;
                    if(fiftyfifty())
                        amount = dice(1,3,0);
                    else
                        amount = 1;
                    player->incstat(sSoul, amount);
                    display->messagec(COLOR_GOOD, "Congratulations! You feel that there is hope for your soul after all. (Soul +%d)", amount);
                } else {
                    display->messagec(COLOR_BOOK, "The tome is quite academic in style, and obviously not meant to be understandable for laymen.");
                    display->messagec(COLOR_BOOK, "You gain nothing from reading the long sentences with too many commas and obscure words");
                    display->messagec(COLOR_BOOK, "which overpopulate this book.");
                }
                display->message(" ");
                break;
            default:
                display->messagec(COLOR_BOOK, "The only book which looks interesting is unfortunately written in some language you don't even recognize.");
                display->message(" ");
                break;
        }
    } else {
        display->messagec(COLOR_BOOK, "After a while, you conclude that there's nothing here that interests you.");
        display->message(" ");
    }
    activated = true;
}

void Cell::activate()
{
    switch(this->type) {
        case cell_bookcase:
            this->activate_bookcase();
            player->moved();
            break;
        default:
            break;
    }
    display->touch();
}

void Cell::set_type(cell_type t)
{
    switch(t) {
        case wall:
            this->set_wall();
            break;
        case floor:
            this->set_floor();
            break;
        case cell_bookcase:
            this->set_bookcase();
            break;
        case cell_chair:
            this->set_chair();
            break;
        case cell_table:
            this->set_table();
            break;
        case cell_coffin:
            this->set_coffin();
            break;
        case cell_candle:
            this->set_candle();
            break;
        case cell_corpse:
            break;
        case stairs_up:
            this->set_stairs_up();
            break;
        case stairs_down:
            this->set_stairs_down();
            break;
        case door_closed:
            this->set_door_closed();
            break;
        case door_open:
            this->set_door_open();
            break;
        default:
            dbg("unknown type!");
            break;
    }
}


/*
 * class: Area
 */
Area::Area()
{
    cell = new Cell*[AREA_MAX_X];

    for(int i = 0; i < AREA_MAX_X; ++i)
        cell[i] = new Cell[AREA_MAX_Y];

    tcodmap = new TCODMap(AREA_MAX_X, AREA_MAX_Y);
    bsp = new TCODBsp(1, 1, AREA_MAX_X, AREA_MAX_Y);
    lights_on = true;
}

Area::~Area()
{
    for(int i = 0; i < AREA_MAX_X; ++i)
        delete [] cell[i];

    delete [] cell;
    delete tcodmap;
    delete bsp;
}

void Area::build_tcodmap()
{
    int x, y;

    for(x = 0; x < AREA_MAX_X; ++x) {
        for(y = 0; y < AREA_MAX_Y; ++y) {
            tcodmap->setProperties(x, y, cell[x][y].is_transparent(), cell[x][y].is_walkable());
        }
    }
}

void Area::set_all_invisible()
{
    int x, y;

    for(x = 0; x < AREA_MAX_X; ++x) {
        for(y = 0; y < AREA_MAX_Y; ++y) {
            cell[x][y].set_visibility(false);
        }
    }
}

void Area::set_all_visible()
{
    int x, y;

    for(x = 0; x < AREA_MAX_X; ++x) {
        for(y = 0; y < AREA_MAX_Y; ++y) {
            this->tcodmap->setProperties(x, y, true, cell[x][y].is_walkable());
            this->cell[x][y].seen();
        }
    }
    player->setfovradius(0);
}

void Area::update_visibility()
{
    int x, y;

    for(x = 0; x < AREA_MAX_X; ++x) {
        for(y = 0; y < AREA_MAX_Y; ++y) {
            if(this->tcodmap->isInFov(x, y))
                cell[x][y].set_visibility(true);
            else
                cell[x][y].set_visibility(false);
        }
    }
}

direction Area::generate_starting_room()
{
    int x1, x2, y1, y2;
    direction d;

    x1 = PLAYER_STARTX - 3;
    x2 = PLAYER_STARTX + 3;
    y1 = PLAYER_STARTY + 1;
    y2 = PLAYER_STARTY - 6;
    make_room(x1, y1, x2, y2);

    d = (direction) d(1, 4);

    if(d == north || d == south) {
        int dx, dy;
        dx = ri(x1+1, x2-1);
        dy = y2;
        make_door(dx, dy, false);
    }
    /*if(d == south) {
      int dx, dy;
      dx = ri(x1, x2);
      dy = y1;
      make_door(dx, dy, false);
      }*/
    if(d == west) {
        int dx, dy;
        dx = x1;
        dy = ri(y1-2, y2+1);
        make_door(dx, dy, false);
    }
    if(d == east) {
        int dx, dy;
        dx = x2;
        dy = ri(y1-2, y2+1);
        make_door(dx, dy, false);
    }

    return d;
}

void Area::generate(area_id_type identifier)
{
    //int j;
    MyCallback *callback = new MyCallback;
    world->a = &world->area[(int)identifier];
    set_id(identifier);
    bsp->splitRecursive(NULL, ri(4,6), ri(4,6), ri(5,8), 1.7f, 1.7f);
    /*int i = ri(1,3);
    if(i==1)
        bsp->traversePreOrder(callback, NULL);
    if(i==2)
        bsp->traverseLevelOrder(callback, NULL);
    if(i==3)
        bsp->traverseInvertedLevelOrder(callback, NULL);
        */

    bsp->traversePreOrder(callback, NULL);
    this->frame();

    /*j = ri(3,6);
    for(int i=0;i<j;i++) 
        this->vertical_line(ri(5, AREA_MAX_Y-5), ri(5, AREA_MAX_Y-5), ri(5, AREA_MAX_X), floor);
    j = ri(3,6);
    for(int i=0;i<j;i++) 
        this->horizontal_line(ri(5, AREA_MAX_X-5), ri(5, AREA_MAX_X-5), ri(5, AREA_MAX_Y), floor);
        */

    this->build_tcodmap();
    lights_on = false;

    this->place_furniture();
    spawn_items(10);

    delete callback;
}

void Area::place_furniture()
{
    int dx, dy, i;
    // bookcases
    for(i = 0; i < 10; ++i) {
        if(fiftyfifty()) {
            int dx,dy;
            coord_t c = world->get_random_floor_cell(this->get_id());
            this->cell[c.x][c.y].set_bookcase();
            for(dx=-1;dx<=1;dx++) {
                for(dy=-1;dy<=1;dy++) {
                    if(this->cell[c.x+dx][c.y+dy].is_walkable()) {
                        if(ri(1,3) == 1) {
                            this->cell[c.x+dx][c.y+dy].set_bookcase();
                        }
                    }
                }
            }
        }
    }

    for(i = 0; i < 15; ++i) {
        if(fiftyfifty()) {
            coord_t c = world->get_random_floor_cell(this->get_id());
            this->cell[c.x][c.y].set_table();
            for(dx=-1;dx<=1;dx++) {
                for(dy=-1;dy<=1;dy++) {
                    if(this->cell[c.x+dx][c.y+dy].get_type() == floor) {
                        if(fiftyfifty())
                            this->cell[c.x+dx][c.y+dy].set_chair();
                    }
                }
            }
        }
    }

    for(i = 0; i < 5; ++i) {
        if(fiftyfifty()) {
            coord_t c = world->get_random_floor_cell(this->get_id());
            this->cell[c.x][c.y].set_table();
        }
    }

    for(i = 0; i < 5; ++i) {
        if(fiftyfifty()) {
            coord_t c = world->get_random_floor_cell(this->get_id());
            this->cell[c.x][c.y].set_chair();
        }
    }

    for(i = 0; i < 5; ++i) {
        if(fiftyfifty()) {
            coord_t c = world->get_random_floor_cell(this->get_id());
            this->cell[c.x][c.y].set_coffin();
        }
    }

    for(i = 0; i < 10; ++i) {
        if(fiftyfifty()) {
            coord_t c = world->get_random_floor_cell(this->get_id());
            this->cell[c.x][c.y].set_candle();
        }
    }

}

void Area::frame()
{
    horizontal_line(1);
    horizontal_line(AREA_MAX_Y - 2);
    vertical_line(1);
    vertical_line(AREA_MAX_X - 2);
}

void Area::make_room(int x1, int y1, int x2, int y2)
{
    horizontal_line(x1, y1, x2);
    horizontal_line(x1, y2, x2);
    vertical_line(x1, y1, y2);
    vertical_line(x2, y1, y2);
}

void Area::make_room_with_doors(int x1, int y1, int x2, int y2)
{
    horizontal_line(x1, y1, x2);
    horizontal_line(x1, y2, x2);
    vertical_line(x1, y1, y2);
    vertical_line(x2, y1, y2);

    make_door(ri(x1,x2), y1, false);
    make_door(ri(x1,x2), y2, false);
    make_door(x1, ri(y1,y2), false);
    make_door(x2, ri(y1,y2), false);
}

void Area::make_door(int x, int y, bool open)
{
    //dbg("Making door at %d,%d (%s)", x, y, open ? "open" : "closed");

    if(x <= 1)
        return;
    if(y <= 1)
        return;
    //if(cell[x][y].get_type() != wall)
    //    return;

    if(open)
        cell[x][y].set_door_open();
    else
        cell[x][y].set_door_closed();
}

void Area::make_stairs_up()
{
    coord_t co;

    co = this->get_random_floor_cell();
    cell[co.x][co.y].set_stairs_up();
    stairs_up = co;
}

void Area::make_stairs_down()
{
    coord_t co;

    co = this->get_random_floor_cell();
    cell[co.x][co.y].set_stairs_down();
    stairs_down = co;
}

coord_t Area::get_random_floor_cell()
{
    coord_t co;

again:
    co.x = ri(1, AREA_MAX_X-2);
    co.y = ri(1, AREA_MAX_Y-2);
    while(this->cell[co.x][co.y].get_type() != floor) {
        co.x = ri(1, AREA_MAX_X-2);
        co.y = ri(1, AREA_MAX_Y-2);
    }

    if(this->cell[co.x][co.y].get_type() == floor)
        return co;
    else
        goto again;
}

coord_t Area::get_random_empty_floor_cell()
{
    coord_t co;

again:
    co.x = ri(1, AREA_MAX_X-2);
    co.y = ri(1, AREA_MAX_Y-2);
    while(this->cell[co.x][co.y].get_type() != floor ) {
        co.x = ri(1, AREA_MAX_X-2);
        co.y = ri(1, AREA_MAX_Y-2);
    }

    if(this->cell[co.x][co.y].get_type() == floor && this->cell[co.x][co.y].item == 0)
        return co;
    else
        goto again;
}

void Area::horizontal_line(int y)
{
    for(int i = 0; i < AREA_MAX_X; ++i)
        cell[i][y].set_wall();
}

void Area::horizontal_line(int x, int y, int x2)
{
    if(x2 > x) {
        for(int j = x; j <= x2; ++j)
            cell[j][y].set_wall();
    } else {
        for(int j = x; j >= x2; --j)
            cell[j][y].set_wall();
    }
}

void Area::horizontal_line(int x, int y, int x2, cell_type t)
{
    if(x2 > x) {
        for(int j = x; j <= x2; ++j)
            cell[j][y].set_type(t);
    } else {
        for(int j = x; j >= x2; --j)
            cell[j][y].set_type(t);
    }
}

void Area::vertical_line(int x, int y, int y2)
{
    if(y2 > y) {
        for(int j = y; j <= y2; ++j)
            cell[x][j].set_wall();
    } else if(y2 < y) {
        for(int j = y; j > y2; --j)
            cell[x][j].set_wall();
    }
}

void Area::vertical_line(int x)
{
    for(int j = 0; j < AREA_MAX_Y; ++j)
        cell[x][j].set_wall();
}

void Area::vertical_line(int x, int y, int y2, cell_type t)
{
//    dbg("vertical line x=%d y=%d y2=%d t=%d", x, y, y2, (int) t);
    if(y2 > y) {
        for(int j = y; j <= y2; ++j)
            cell[x][j].set_type(t);
    } else if(y2 < y) {
        for(int j = y; j > y2; --j)
            cell[x][j].set_type(t);
    }
}

bool Area::cell_is_visible(int x, int y)
{
    return tcodmap->isInFov(x, y);
    //return cell[x][y].is_visible();
}

const char *Area::get_area_name()
{
    if(id >= 0 && id <= MAX_AREAS)
        return area_name[(int) id];
    else
        return NULL;
}

bool Area::is_walkable(int x, int y)
{
    return this->cell[x][y].is_walkable();
}

void Area::spawn_items(int num)
{
    int i = 0;
    vector<Item>::iterator it; 
    it = g.itemdef.begin();

    while(i != num) {
       int c = ri(1, 100);
       //cout << "c = " << c << " chance = " << it->chance << endl;
       if(c <= it->chance) {
           //cout << i << " - spawning a " << it->get_name() << endl;

           coord_t co;
           Item newitem;
           
           co = this->get_random_empty_floor_cell();
           newitem = *it;
           newitem.co = co;

           world->items.push_back(newitem); // master list of items -- TODO: needed???
           //this->items.push_back(*it);  // master list of items in one area - TODO: needed?

           this->cell[co.x][co.y].item = new Item;
           *this->cell[co.x][co.y].item = newitem;
           this->cell[co.x][co.y].item->fg = TCODColor::darkGreen;
           i++;
       } else {
           if(it == g.itemdef.end())
               it = g.itemdef.begin();
           else
               it++;
       }
    }
}


/*
 * class: World
 */
World::World()
{
    area = new Area[MAX_AREAS];
    current_area = 0;
    //a = &area[current_area];
}

World::~World()
{
    delete [] area;
}

bool World::is_walkable(Area *where, int x, int y)
{
    return where->cell[x][y].is_walkable();
}

bool World::is_closed_door(Area *where, int x, int y)
{
    if(where->cell[x][y].get_type() == door_closed)
        return true;
    else
        return false;
}

bool World::is_open_door(Area *where, int x, int y)
{
    if(where->cell[x][y].get_type() == door_open)
        return true;
    else
        return false;
}

void World::open_door(Area *where, int x, int y)
{
    where->cell[x][y].set_door_open();
    where->build_tcodmap();
}

void World::close_door(Area *where, int x, int y)
{
    where->cell[x][y].set_door_closed();
    where->build_tcodmap();
}

bool World::close_nearest_door(Actor *actor)
{
    // Close the door nearest to actor, start at northwest.
    // Returns true if successfully closed a door.

    int x, y, dx, dy;

    x = actor->getx();
    y = actor->gety();
    for(dx=-1; dx <= 1; dx++) {
        for(dy=-1; dy <= 1; dy++) {
            if(is_open_door(actor->area, x+dx, y+dy)) {
                close_door(actor->area, x+dx, y+dy);
                if(actor == player)
                    player->moved();
                return true;
            }
        }
    }

    return false;
}

void World::draw_map()
{
    int i, j;

    player->area->tcodmap->computeFov(player->getx(), player->gety(), player->getfovradius(), true, FOV_BASIC);
    for(i = 1; i < AREA_MAX_X-1; ++i) {
        for(j = 1; j < AREA_MAX_Y-1; ++j) {
            if(player->can_see(i, j) || player->area->cell[i][j].is_seen()) {
                draw_cell(i, j);
            } else {
                if(!player->area->lights_on) {
                    draw_cell(i, j, TCODColor::black, TCODColor::black); 
                }
            }

        }
    }
    display->touch();
}

void World::draw_cell(int x, int y)
{
    if(player->area->cell[x][y].inhabitant) {
        if(player->area->cell[x][y].inhabitant->alive) {
            player->area->cell[x][y].inhabitant->draw();
        }
    } else {
        if(player->area->cell[x][y].item) {
            display->putmap(x, y, player->area->cell[x][y].item->c, player->area->cell[x][y].item->fg, player->area->cell[x][y].item->bg);
        } else {
            display->putmap(x, y, a->cell[x][y].c, a->cell[x][y].fg, a->cell[x][y].bg);
        }
    }
}

void World::draw_cell(coord_t co)
{
    display->putmap(co.x, co.y, a->cell[co.x][co.y].c, a->cell[co.x][co.y].fg, a->cell[co.x][co.y].bg);
}

void World::draw_cell(int x, int y, TCODColor fg, TCODColor bg)
{
    display->putmap(x, y, player->area->cell[x][y].c, fg, bg);
}

void World::update_fov()
{
    coord_t co;

    co = player->getxy();
    player->area->tcodmap->computeFov(co.x, co.y, player->getfovradius(), true, FOV_BASIC);
}

coord_t World::get_random_walkable_cell(area_id_type id)
{
    coord_t co;

again:
    co.x = ri(1, AREA_MAX_X-2);
    co.y = ri(1, AREA_MAX_Y-2);
    while(!area[id].cell[co.x][co.y].is_walkable()) {
        co.x = ri(1, AREA_MAX_X-2);
        co.y = ri(1, AREA_MAX_Y-2);
    }

    if(area[id].cell[co.x][co.y].is_walkable())
        return co;
    else
        goto again;
}

coord_t World::get_random_floor_cell(area_id_type id)
{
    coord_t co;

again:
    co.x = ri(1, AREA_MAX_X-2);
    co.y = ri(1, AREA_MAX_Y-2);
    while(area[(int)id].cell[co.x][co.y].get_type() != floor) {
        co.x = ri(1, AREA_MAX_X-2);
        co.y = ri(1, AREA_MAX_Y-2);
    }

    if(area[(int)id].cell[co.x][co.y].get_type() == floor)
        return co;
    else
        goto again;
}

void World::set_inhabitant(Actor *actor)
{
    actor->area->cell[actor->getx()][actor->gety()].inhabitant = actor;
    actor->area_id = actor->area->get_id();
}

void World::clear_inhabitant(Area *where, coord_t co)
{
    where->cell[co.x][co.y].inhabitant = NULL;
    /*if(where->cell_is_visible(co.x, co.y))
        where->cell[co.x][co.y].draw(co.x, co.y);
    else
        where->cell[co.x][co.y].draw(co.x, co.y, TCODColor::black, TCODColor::black);*/
}

const char *World::get_cell_type(Area *where, int x, int y)
{
    switch(where->cell[x][y].get_type()) {
        case floor: return "floor"; break;
        case wall: return "wall"; break;
        case nothing: return "nothing"; break;
        case door_open: return "open door"; break;
        case door_closed: return "closed door"; break;
        default: return "unknown"; break;
    }
}

cell_type World::get_cell_type(Area *where, coord_t co)
{
    return where->cell[co.x][co.y].get_type();
}

void World::generate_stairs()
{
    int i;

    for(i=1; i<=10; i++) {
        world->area[i].make_stairs_down();
        world->area[i].make_stairs_up();
    }

    world->area[0].make_stairs_up();
    world->area[11].make_stairs_down();
}




// vim: fdm=syntax guifont=Terminus\ 8
