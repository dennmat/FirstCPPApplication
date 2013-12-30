#include "stdafx.h"

#include <iostream>
#include <sstream>

#include "Person.h"

#include "libtcod.hpp"

#include "thinker.h"
#include "inventory.h"
#include "equipment.h"
#include "Representation.h"
#include "tile.h"	
#include "attribute.h"
#include "attribute_container.h"
#include "combat.h"

Person::Person(std::string name, int age, int x, int y, char repr, std::string combat_name)
{
    this->name = name;
    this->age = age;
    this->x = x;
    this->y = y;

    //set its representation, aka color and char
    this->representation = new Representation;
    this->representation->repr = repr;
    this->representation->setFGColor(TCODColor::lighterFlame, true, true, true);

    Inventory* inventory =   new Inventory;
    this->inventory = inventory;
    this->equipment = new Equipment;
    this->equipment->master = this;

    is_fighter = true;

    combat = new Combat("temp name", 50, this, 't' );
    combat->assign_to_master(this);
    has_live_combat = true;

    my_tile = NULL;

    is_active = true;

};

Person::Person() : Actor()
{
    this->name = "Unset Person name";
    this->age = 80085;
    this->x = 10;
    this->y = 10;

    this->dest_x = -1;
    this->dest_y = -1;


    //set its representation, aka color and char
    this->representation->repr = '~';

    Inventory* inventory =   new Inventory;
    this->equipment = new Equipment;
    this->equipment->master = this;

    is_fighter = true;

    combat = new Combat("temp2 name", 25, this, 't');
    combat->assign_to_master(this);
    has_live_combat = true;

    my_tile = NULL;
};

Person::~Person(){
    //TODO: safely remove Combat
};

void Person::update(Game* game)
{
    //apply item basic item attributes of only equipped items
    this->equipment->Update();

    if (this->thinker != NULL)
    {
        thinker->update(game);

        //Combat UPDATE
        if (has_live_combat == true) {
            combat->update(game);
        }
        else {
            printf("no combat\n");
        }
    }
};

void Person::attack(Actor * target)
{
    combat->Attack(((Person*)target)->combat, 10); //shit will happen if the target isn't a Person
};
