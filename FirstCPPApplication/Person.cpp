#include "stdafx.h"
#include "person.h"
#include "world.h"

void Person::putPerson(Tile * current_tile, Tile * next_tile, int new_x, int new_y)
{	//puts a person on a tile, resets the old tile

    if (current_tile != NULL){
    current_tile->is_occupied = false; //TODO: Check if current tile is occupied by someone other than self
    current_tile->occupant = NULL;
    }
    
    if (next_tile != NULL){
    next_tile->is_occupied = true; 
    next_tile->occupant = this;
    }

    x = new_x;
    y = new_y;

};

