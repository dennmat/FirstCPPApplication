#include "stdafx.h"
#include "tile.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

Tile::Tile()
{
    tiletype = 0;
    _is_occupied = false;

    occupants = new std::vector<Actor*>;

    updateTileType(tiletype);
};

void Tile::makeOccupied(Actor* the_actor)
{
    occupant = the_actor;
    occupants->push_back(the_actor);

    if (occupants->size() > 0)
    {
        _is_occupied = true;
    }
    else
    {
        cout << "tile ain't occupied" << endl;
    };
};

void Tile::makeUnoccupied(Actor* the_actor)
{
    //remove the actor from vector
    // for(std::vector<Actor*>::iterator it = occupants->begin(); it != occupants->end(); ++it) {
    for(std::vector<Actor*>::size_type i = 0; i != occupants->size(); i++) 
    {
        if (occupants->at(i) == the_actor)
        {
            //cout << "ASDADSAD: " << occupants->size() << endl;
            occupants->erase(occupants->begin() + i);
            //cout << "post ASDADSAD: " << occupants->size() << endl;
            break;
        }
    }

    //see if anyone else is left on the tile
    if (occupants->size() == 0)
    {
        _is_occupied = false;
        occupant = NULL;
    }
    else
    {
        //make the last actor on top of the tile
        occupant = occupants->back();
        cout << "tile ain't empty" << endl;
    };
};

void Tile::updateTileType(int type )
{
    tiletype = type;

    if (type == 0) { tile = new BaseTileType; }
    else if (type == 3) { tile = new FloorTileType; }
    else if (type == 1) { tile = new WallTileType; }
    else if (type == 2) { tile = new WarpTileType; }
    else 
    {
        cout << type << endl;
        cout << "^ INVALID TILETYPE OH MY GOD" << endl; //probably because the tiletype is being assigned with a `new` call.
    }

};
