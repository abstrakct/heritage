/*
 * Generic actor class definition
 *
 * Copyright (C) 2013
 */

using namespace std;
#include "common.h"
#include "actor.h"
#include "player.h"
#include "display.h"
#include "game.h"
#include "world.h"

extern World *world;
extern Display *display;
extern Player *player;
extern Game g;

const char *special_name[] = {
    "(none)",
    "Mindblast",
    "Soulcrush",
    "Powerfist",
    "Special 4",
    "Special 5",
    "Special 6",
    "Special 7",
    "Special 8",
    "Special 9",
};

const char *bodypart_string[] = {
    "head",
    "chest",
    "left arm",
    "right arm",
    "left foot",
    "right foot",
    "guts",
    "face",
    "groin",
};

const char *sanitydesc[] = {
    "Member of WBC",              //  0 - 3
    "Completely insane",          //  3 - 10
    "Raving and drooling",        // 11 - 20
    "Nervous breakdown imminent", // 21 - 30
    "You're losing it",           // 31 - 40
    "Average man in the street",  // 41 - 60
    "Less insane than most",      // 61 - 70
    "Still doing great",          // 71 - 80
    "Pretty darn sane",           // 81 - 90 
    "Completely sane",            // 91 - 99
    "Mental health poster child", // 100
};

Actor::Actor()
{
    role = role_unknown;
    c = ' ';
    co.x = co.y = 0;
    alive = true;
    enemy = NULL;
    moved_ = false;
}

bool Actor::is_male()
{ 
    if(male)
        return true;
    else
        return false;
};

void Actor::kill()
{
    //display->message("%s dies at %d,%d!", name, this->co.x, this->co.y);
    if(!this->is_player()) {
        if(player->can_see(this))
            display->messagec(COLOR_FATAL, "%s has been killed!", this->getname());
        else
            display->messagec(COLOR_FATAL, "You hear a horrible, chilling scream!");
        player->incfear();
        player->incfear();
        this->alive = false;
        this->area->cell[this->co.x][this->co.y].set_corpse(this);
        this->area->cell[this->co.x][this->co.y].inhabitant = NULL;
    } else {
        player->die();
    }
}

void Actor::setxy(int x, int y)
{
    co.x = x;
    co.y = y;
}

void Actor::setxy(coord_t newco)
{
    //dbg("setting co to %d,%d (type of cell is %s)", newco.x, newco.y, world->get_cell_type(newco.x, newco.y));
    co = newco;
}

int Actor::getx() 
{
    return co.x;
}

int Actor::gety() 
{
    return co.y;
}

void Actor::setprevxy(int x, int y)
{
    prev.x = x;
    prev.y = y;
}

void Actor::setprevxy(coord_t newco)
{
    prev = newco;
}

void Actor::setchar(char newc)
{
    c = newc;
}

void Actor::setname(const char *name)
{
    strcpy(this->name, name);
    //        dbg("set name %s", name);
}

void Actor::setcolors(TCODColor fg, TCODColor bg)
{
    this->fg = fg;
    this->bg = bg;
}

const char *Actor::getname() const
{
    return this->name;
}

const coord_t Actor::getxy() const
{
    return co;
}

void Actor::draw()
{
    if(world->a->cell_is_visible(co.x, co.y)) {
        //       if(prev.x != co.x || prev.y != co.y || (prev.x != co.x && prev.y != co.y))
        //               world->draw_cell(this->prev);
        display->putmap(this->co.x, this->co.y, this->c, this->fg, this->bg);
        display->touch();
    }
}

void Actor::draw(TCODColor fg, TCODColor bg)
{
    if(world->a->cell_is_visible(co.x, co.y)) {
        //if(prev.x != co.x || prev.y != co.y || (prev.x != co.x && prev.y != co.y))
        //        world->draw_cell(this->prev);
        display->putmap(this->co.x, this->co.y, this->c, fg, bg);
        display->touch();
    }
}

void Actor::drawcorpse()
{
    display->putmap(this->co.x, this->co.y, 0xB6);
    display->touch();
}

void Actor::move(int dx, int dy)
{
    if(this->area->cell[this->co.x + dx][this->co.y + dy].inhabitant) {
        if(this == player) {
            display->message("%s is standing in your way!", this->area->cell[this->co.x + dx][this->co.y + dy].inhabitant->getname());
            display->message("Do you want to attack %s [y/n]?", this->area->cell[this->co.x + dx][this->co.y + dy].inhabitant->getname());
            display->touch();
            display->print_messages();
            display->touch();
            if(display->askyn()) {
                player->attack(this->area->cell[this->co.x + dx][this->co.y + dy].inhabitant);
                player->moved(true);
            } else {
                player->moved(false);
            }
        } else {
            if(this->enemy) {
                if(this->enemy->area == this->area) {
                    if(this->enemy->getx() == this->co.x + dx && this->enemy->gety() == this->co.y + dy) {
                        attack(enemy);
                        return;
                    }
                }
            }
        }

        return;
    }

    if(world->is_closed_door(this->area, this->co.x + dx, this->co.y + dy)) {
        world->open_door(this->area, this->co.x + dx, this->co.y + dy);
        display->touch();
        return;
    }

    if(g.wizmode && this == player) {
        prev = co;
        co.x += dx;
        co.y += dy;
        world->clear_inhabitant(this->area, prev);
        world->set_inhabitant(this);
        player->moved(true);
        display->touch();
        return;
    }

    if(this->area->is_walkable(this->co.x + dx, this->co.y + dy)) {
        prev = co;
        co.x += dx;
        co.y += dy;
        world->clear_inhabitant(this->area, prev);
        world->set_inhabitant(this);
        display->touch();
        if(this == player)
            player->moved(true);
    }
}

void Actor::move_left()
{
    this->move(-1, 0);
}

void Actor::move_right()
{
    this->move(1, 0);
}

void Actor::move_down()
{
    this->move(0, 1);
}

void Actor::move_up()
{
    this->move(0, -1);
}

void Actor::move_nw()
{
    this->move(-1, -1);
}

void Actor::move_ne()
{
    this->move(1, -1);
}

void Actor::move_sw()
{
    this->move(-1, 1);
}

void Actor::move_se()
{
    this->move(1, 1);
}

const char *Actor::get_sanitydesc()
{
    if(getstat(sSanity) == 100)
        return sanitydesc[10];
    else if(getstat(sSanity) >= 91)
        return sanitydesc[9];
    else if(getstat(sSanity) >= 81)
        return sanitydesc[8];
    else if(getstat(sSanity) >= 71)
        return sanitydesc[7];
    else if(getstat(sSanity) >= 61)
        return sanitydesc[6];
    else if(getstat(sSanity) >= 41)
        return sanitydesc[5];
    else if(getstat(sSanity) >= 31)
        return sanitydesc[4];
    else if(getstat(sSanity) >= 21)
        return sanitydesc[3];
    else if(getstat(sSanity) >= 11)
        return sanitydesc[2];
    else if(getstat(sSanity) >= 3)
        return sanitydesc[1];
    else 
        return sanitydesc[0];
}

int Actor::getstat(enum_stat which)
{
    return this->stat[(int)which];
}

void Actor::setstat(enum_stat which, int what)
{
    this->stat[which] = what;
}

void Actor::decstat(enum_stat which, int amount)
{
    if(this->stat[which] > 0)
        this->stat[which] -= amount;
}

void Actor::incstat(enum_stat which, int amount)
{
    this->stat[which] += amount;
    if(which == sBody || which == sMind || which == sSoul) {
        if(this->stat[which] > 20)
            this->stat[which] = 20;
    } 
}

void Actor::decsanity()
{
    // Decrease sanity, if appropriate.

    if(fiftyfifty()) {
        int amount = 1;

        amount -= ability_modifier(getstat(sMind));
        amount += getstat(sFear) / 20;
        if(amount <= 0)
            amount = 1;
        decstat(sSanity, amount);
        display->messagec(COLOR_FEAR, "You feel %s.", fiftyfifty() ? "less sane" : "a little out of whack");
    }
}

void Actor::incfear()
{
    // Increase fear, if appropriate.

    if(fiftyfifty()) {
        int amount = 1;

        amount -= ability_modifier(getstat(sMind));
        amount += getstat(sSanity) / 20;
        if(amount <= 0)
            amount = 1;
        incstat(sFear, amount);
        display->messagec(COLOR_FEAR, "You feel %s.", fiftyfifty() ? "scared" : "afraid");
    }

    if(getstat(sFear) > 80 && this->is_player() && fiftyfifty()) {
        display->messagec(COLOR_FEAR, "You are %s!", fiftyfifty() ? "very afraid" : "terrified");
    }

    if(getstat(sFear) > 100 && this->is_player()) {
        display->messagec(COLOR_FATAL, "You have been scared to death!");
        player->kill();
    }
}

void Actor::decfear()
{
    // Decrease fear, if appropriate.

    if(getstat(sFear) > 0) {
        if(fiftyfifty()) {
            int amount = 1;

            amount += ability_modifier(getstat(sMind));
            amount -= getstat(sSanity) / 20;
            if(amount <= 0)
                amount = 1;
            decstat(sFear, amount);
            if(ri(1,10) == 10)
                display->messagec(COLOR_LESSFEAR, "You feel %s.", fiftyfifty() ? "a little bit more relaxed" : "a little less afraid");
        }
    }
}

void Actor::use_stairs()
{
}

void Actor::set_in_combat()
{
}

bool Actor::pass_roll(enum_stat stat)
{
    int x;

    x = dice(1,21,0);
    if(x <= this->getstat(stat))
        return true;
    else
        return false;
}

void Actor::attack_physical(Actor *target)
{
    int tohit = target->getstat(sBody);
    int d = dice(1, 20, 0);
    if(d >= tohit) {
        int damage = dice(1, this->getstat(sBody), ability_modifier(this->getstat(sBody)));
        if(damage <= 0)
            damage = 1;
        if(this != player) {
            if(!this->can_see(target)) {
            //if(!world->a->tcodmap->isInFov(target->getx(), target->gety())) {
                int x = ri(1,10);
                switch(x) {
                    case  1: display->messagec(COLOR_FEAR, "You hear a scream somewhere in the house."); player->incfear(); break;
                    case  2: display->messagec(COLOR_FEAR, "You hear the sounds of fighting somewhere in the house."); player->incfear(); break;
                    case  3: display->messagec(COLOR_FEAR, "You hear someone shout."); player->incfear(); break;
                    case  4: display->messagec(COLOR_FEAR, "You hear someone yell."); player->incfear(); break;
                    case  5: display->messagec(COLOR_FEAR, "You hear the sound of something breaking."); player->incfear(); break;
                    case  6: display->messagec(COLOR_FEAR, "You hear the sound of someone crying."); player->incfear(); break;
                    case  7: display->messagec(COLOR_FEAR, "You hear someone wailing."); player->incfear(); break;
                    case  8: display->messagec(COLOR_FEAR, "You hear a bloodcurdling shriek from somewhere in the house."); player->incfear(); break;
                    case  9: display->messagec(COLOR_FEAR, "You hear a horrible howl from somewhere in the house."); player->incfear(); break;
                    case 10: display->messagec(COLOR_FEAR, "You hear weird, muffled noises from somewhere in the house."); player->incfear(); break;
                    default: break;
                }
            } else {
                if(target != player)
                    display->messagec(COLOR_FEAR, "You see %s attacking %s!", this->getname(), target->getname());
                if(target == player)
                    display->messagec(COLOR_FEAR, "%s %s you in the %s!", this->getname(), fiftyfifty() ? "hits" : "kicks", bodypart_string[ri(0,8)]);
                player->incfear();
            }
        } else {    // this == player
            display->messagec(COLOR_GOOD, "You attack %s, causing %d amounts of damage!", target->getname(), damage);
        }
        //display->message("%s HIT %s! %d damage.", this->name, target->getname(), damage);
        target->decstat(sHealth, damage);
        if(target->getstat(sHealth) <= 0) {
            target->kill();
            this->enemy = NULL;

        }
    }
}

void Actor::attack(Actor *target, attack_type type)
{
    target->enemy = this;
    if(this == player) {
        target->set_in_combat();
    }

    switch(type) {
        case body:
            attack_physical(target);
            break;
        default:
            break;
    }
}

bool Actor::can_see(Actor *target)
{
    bool result;

    result = this->area->tcodmap->isInFov(target->getx(), target->gety());
    if(result && this == player)
        this->area->cell[target->getx()][target->gety()].seen();

    return result;
}

bool Actor::can_see(int x, int y)
{
    bool result;

    result = this->area->tcodmap->isInFov(x, y);
    if(result && this == player)
        this->area->cell[x][y].seen();

    return result;
}

bool Actor::is_next_to(Actor *target)
{
    int dx,dy;
    for(dx=-1;dx<=1;++dx) {
        for(dy=-1;dy<=1;++dy) {
            if((this->getx() + dx == target->getx()) && (this->gety() + dy == target->gety()))
                return true;
        }
    }

    return false;
}

int  Actor::add_special_attack(special_type t)
{
    vector<SpecialAttack>::iterator it;
    SpecialAttack *s;

    for(it = this->special.begin(); it != this->special.end(); ++it) {
        if(it->type == t) {
            it->level++;
            return SPECIAL_ADD_INCREASE;
        }
    }

    s = new SpecialAttack(t);
    this->special.push_back(*s);
    delete s;
    return SPECIAL_ADD_SUCCESS;
}

SpecialAttack::SpecialAttack()
{
    type = special_none;
    level = 0;
    name[0] = '\0';
}

SpecialAttack::SpecialAttack(special_type t)
{
    type = t;
    level = 1;
    strcpy(name, special_name[(int) t]);
}
// vim: fdm=syntax guifont=Terminus\ 8
