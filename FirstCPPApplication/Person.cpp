#include "stdafx.h"
#include "person.h"
#include "world.h"

#include <iostream>
// #include <sstream>
// #include <fstream>

Person::Person(){
    is_fighter = true;
    pet = new Pet;
};
Person::~Person(){
    //TODO: safely remove pet
};

void Person::putPerson(Tile * current_tile, Tile * next_tile, int new_x, int new_y)
{	//puts a person on a tile, resets the old tile

    if (current_tile != NULL){
		current_tile->makeUnoccupied(); //TODO: Check if current tile is occupied by someone other than self
        current_tile->occupant = NULL;
    }

    if (next_tile != NULL){
        next_tile->makeOccupied() ; 
        next_tile->occupant = this;
        // cout << "new occupant: " << name << endl;
    }
    else if (next_tile == NULL)
    {
        cout << "next tile is null" << endl;
    }

    x = new_x;
    y = new_y;

};


void Person::attack(Person * target)
{

    pet->Attack(target->pet, 10);

};
