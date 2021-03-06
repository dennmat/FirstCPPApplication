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
#include "messages.h"
#include "game.h"
#include "ui.h"

Person::Person(std::string name, int age, int x, int y, char repr, std::string combat_name)
{
    this->cls_name = "Person";
    this->name = name;
    this->age = age;
    this->x = x;
    this->y = y;

    if (name == "Josh")
	{
    new Message(Ui::msg_handler_main, "Creating the Josh player");
	}

    this->xp = 0;
    this->xp_this_level = 0;
    this->xp_required = 100;
    this->xp_value = 10;
    this->level = 1;

    //set its representation, aka color and char
    this->representation = new Representation;
    this->representation->repr = repr;
    this->representation->setFGColor(TCODColor::lighterFlame, true, true, true);

    Inventory* inventory =   new Inventory;
    this->inventory = inventory;
    this->inventory->master = this;
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
    this->cls_name = "Person";
    this->name = "Unset Person name";
    this->age = 80085;
    this->x = 10;
    this->y = 10;

    this->dest_x = -1;
    this->dest_y = -1;


    //set its representation, aka color and char
    this->representation->repr = '~';

    Inventory* inventory =   new Inventory;
    this->inventory = inventory;
    this->inventory->master = this;
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

void Person::update()
{
    //apply item basic item attributes of only equipped items if they've not
    //been equipped already
    this->equipment->Update();

    //apply regen
    this->attrs->Update();

    //TODO intervals

    if (this->thinker != NULL)
    {
        thinker->update();

        //Combat UPDATE
        if (has_live_combat == true) {
            combat->update();
        }
        else {
            printf("no combat\n");
        }
    }
};

void Person::attack(Actor * target)
{
    //only Persons and children have combats
    combat->Attack(((Person*)target)->combat, this->attrs->damage->current_val); 
};
