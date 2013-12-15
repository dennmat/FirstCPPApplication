#include "stdafx.h"
#include "skeleton.h"

#include <iostream>
#include <sstream>

#include "libtcod.hpp"

#include "Person.h"
#include "thinker.h"
#include "inventory.h"
#include "equipment.h"
#include "Representation.h"
#include "tile.h"	
#include "attribute.h"
#include "attribute_container.h"
#include "combat.h"

Skeleton::Skeleton(std::string name, int age, int x, int y, char repr, std::string combat_name)
{
    this->name = name;
    this->age = age;
    this->x = x;
    this->y = y;

    //set its representation, aka color and char
    Representation * new_repr =   new Representation;
    this->representation = new_repr;
    this->representation->repr = repr;
    this->representation->setFGColor(TCODColor::white, true, true, true);

    Inventory* inventory =   new Inventory;
    this->inventory = inventory;
    this->equipment = new Equipment;
    this->equipment->master = this;

    is_fighter = true;

    combat = new Combat("temp name", 50, this, 't' );
    combat->assign_to_master(this);
    has_live_combat = true;

    my_tile = NULL;

};
