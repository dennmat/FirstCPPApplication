#include "tile.h"

Tile::Tile()
{
    tiletype = 0;
    _is_occupied = false;

    updateTileType(tiletype);
};

void Tile::makeOccupied()
{
    _is_occupied = true;
};

void Tile::makeUnoccupied()
{
    //TODO: Check if current tile is occupied by someone 
    //if (
    _is_occupied = false;
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
