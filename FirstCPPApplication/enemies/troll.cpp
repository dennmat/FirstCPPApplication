#include "stdafx.h"
#include "troll.h"

#include <iostream>
#include <sstream>

#include "libtcod.hpp"

#include <actors/person.h>
#include "thinker.h"
#include "inventory.h"
#include "equipment.h"
#include "Representation.h"
#include "tile.h"	
#include "attribute.h"
#include "attribute_container.h"
#include "combat.h"

Troll::Troll(std::string name, int age, int x, int y, char repr, std::string combat_name) : Person(name, age, x, y, repr, combat_name)
{
    this->xp_value = 100;
    this->cls_name = "Troll";
    // this->name = name;
    // this->age = age;
    // this->x = x;
    // this->y = y;

    //set its representation, aka color and char
    // Representation * new_repr =   new Representation;
    // this->representation = new_repr;
    // this->representation->repr = repr;
    this->representation->setFGColor(TCODColor::darkGreen, true, true, true);

    // Inventory* inventory =   new Inventory;
    // this->inventory = inventory;
    // this->equipment = new Equipment;
    // this->equipment->master = this;

    // is_fighter = true;

    // combat = new Combat("temp name", 50, this, 't' );
    // combat->assign_to_master(this);
    // has_live_combat = true;

    // my_tile = NULL;

    this->attrs->health->max_val = 1;
    this->attrs->health->current_val = 1;

};
