#include "stdafx.h"
#include "person.h"
#include "world.h"

#include <iostream>
// #include <sstream>
// #include <fstream>

Person::Person(){
    is_fighter = true;
    pet = new Pet;
    my_tile = NULL;
};
Person::~Person(){
    //TODO: safely remove pet
};

void Person::putPerson( Tile * next_tile, int new_x, int new_y)
{	//puts a person on a tile, resets the old tile

    if (my_tile != NULL){
        my_tile->makeUnoccupied(); //TODO: Check if current tile is occupied by someone other than self
        my_tile->occupant = NULL;
    }

    if (next_tile != NULL){
        next_tile->makeOccupied() ; 
        next_tile->occupant = this;
        this->my_tile = next_tile;
        // cout << "new occupant: " << name << endl;
    }
    else if (next_tile == NULL)
    {
        cout << "next tile is null" << endl;
    }

    if (new_x != NULL && new_y != NULL)
    {
        x = new_x;
        y = new_y;
    }

};


void Person::attack(Actor * target)
{
    pet->Attack(((Person*)target)->pet, 10); //shit will happen if the target isn't a Person
};
