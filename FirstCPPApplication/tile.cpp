#include "stdafx.h"
#include "tile.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

Tile::Tile()
{
    tiletype = 0;
    _is_occupied = false;

    updateTileType(tiletype);
};

void Tile::makeOccupied()
{
    if (occupants.size() > 0)
    {
    _is_occupied = true;
    }
    else
    {
        cout << "tile ain't occupied" << endl;
    };
};

void Tile::makeUnoccupied()
{
    if (occupants.size() == 0)
    {
    _is_occupied = false;
    }
    else
    {
        cout << "tile ain't empty" << endl;
    };
};

void Tile::updateTileType(int type )
{
    tiletype = type;

    if (type == 0)
    {
        tile = new BaseTileType;
    }
    else if (type == 3)
    {
        tile = new FloorTileType;
    }
    else if (type == 1)
    {
        tile = new WallTileType;
    }
    else if (type == 2)
    {
        tile = new WarpTileType;
    }
    else 
    {
        cout << type << endl;
        cout << "^ INVALID TILETYPE OH MY GOD" << endl; //probably because the tiletype is being assigned with a `new` call.
    }

};
