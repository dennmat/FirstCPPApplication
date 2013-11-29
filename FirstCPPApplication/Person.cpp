#include "stdafx.h"

#include <iostream>
#include <sstream>

#include "Person.h"
#include "thinker.h"
#include "inventory.h"
#include "equipment.h"
#include "Representation.h"
#include "tile.h"	
#include "Pet.h"
// #include <fstream>

Person::Person(std::string name, int age, int x, int y, char repr, std::string pet_name){
    this->name = name;
    this->age = age;
    this->x = x;
    this->y = y;

    //set its representation, aka color and char
    Representation * new_repr =   new Representation;
    this->representation = new_repr;
    this->representation->repr = repr;

    Inventory* inventory =   new Inventory;
    this->inventory = inventory;
    this->equipment = new Equipment;

    is_fighter = true;

    pet = new Pet;
    pet->assign_to_master(this);
    has_live_pet = true;

    my_tile = NULL;

};

Person::Person()
{
    this->name = "Unnamed";
    this->age = 80085;
    this->x = 10;
    this->y = 10;

    this->dest_x = -1;
    this->dest_y = -1;

    //set its representation, aka color and char
    Representation * new_repr =   new Representation;
    this->representation = new_repr;
    this->representation->repr = '~';

    Inventory* inventory =   new Inventory;
    this->inventory = inventory;
    this->equipment = new Equipment;

    is_fighter = true;

    pet = new Pet;
    pet->assign_to_master(this);
    has_live_pet = true;

    my_tile = NULL;
    l_path = NULL;
};

Person::~Person(){
    //TODO: safely remove pet
};

void Person::update(Game* game)
{

    //apply item basic item attributes of only equipped items
    this->equipment->Update();

    if (this->thinker != NULL)
    {
        thinker->update(game);

        //PET UPDATE
        if (has_live_pet == true) {
            pet->update(game);
        }
        else {
            printf("no pet\n");
        }
    }

};



void Person::attack(Actor * target)
{
    pet->Attack(((Person*)target)->pet, 10); //shit will happen if the target isn't a Person
};
