#include "stdafx.h"
#include "tile.h"
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "map.h"
#include "item.h"

using namespace std;

Tile::Tile()
{
    tiletype = 0;
    _is_occupied = false;
    _is_known = false;

    occupants = new std::vector<Actor*>;
    items = new std::vector<Item*>;

    updateTileType(tiletype);
};

void Tile::drawColorsToRoot(TCODColor fg_color, TCODColor bg_color)
{
    // if (fg_color != 0)
    // {
    //     TCODConsole::root->setCharForeground(this->tile_x, this->tile_y, fg_color);
    // };
    // if (bg_color != NULL)
    // {
    //     TCODConsole::root->setCharBackground(this->tile_x, this->tile_y, bg_color);
    // };
};

void Tile::setKnown(bool is_known)
{
    this->_is_known = is_known;
    // cout << "is known" << endl;
};

void Tile::place_item_down(Item* item)
{
    //check for item in list, if its not, add it in
    auto it = std::find(this->items->begin(), this->items->end(), item);
    if (it == this->items->end())
    {
        this->items->push_back(item);
    };

};
void Tile::pick_up_item(Item* item)
{

};

void Tile::makeOccupied(Actor* the_actor)
{
    occupant = the_actor;
    occupants->push_back(the_actor);

    if (occupants->size() > 0)
    {
        _is_occupied = true;
        bool is_trans = map->l_map->isTransparent(tile_x, tile_y);
        map->l_map->setProperties(tile_x, tile_y, is_trans, false);
    }
    else
    {
        cout << "tile ain't occupied" << endl;
    };
};

void Tile::makeUnoccupied(Actor* the_actor)
{
    //remove the actor from vector
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

    //see if anyone else is left on the tile, if not:
    if (occupants->size() == 0)
    {
        _is_occupied = false;
        occupant = NULL;
        bool is_trans = map->l_map->isTransparent(tile_x, tile_y);
        map->l_map->setProperties(tile_x, tile_y, is_trans, true);
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
    else if (type == 1) { tile = new WallTileType; }
    else if (type == 2) { tile = new WarpTileType; }
    else if (type == 3) { tile = new FloorTileType; }
    else if (type == 4) { tile = new DoorTileType; }
    else 
    {
        cout << type << endl;
        cout << "Invalid TILETYPE" << endl; //probably because the tiletype is being assigned with a `new` call.
    }

};

Tile* Tile::getTileAtRelative(int x, int y)
{
    //if tile is in map get tile, otherwise fail nicely
    int new_x = this->tile_x + x;
    int new_y = this->tile_y + y;

    if (new_x <= this->map->width && new_x >= 0 
            && new_y <= this->map->height && new_y >= 0)
    {
        return this->map->getTileAt(new_x, new_y);
    }

    else {

        return NULL;
    };

};

Tile* Tile::getTopLeftTile()
{
    return this->getTileAtRelative(-1, -1);
};

Tile* Tile::getTopMidTile()
{
    return this->getTileAtRelative(0, -1);
};

Tile* Tile::getTopRigtTile()
{
    return this->getTileAtRelative(1, -1);
};

Tile* Tile::getMidRightTile()
{
    return this->getTileAtRelative(1, 0);
};

Tile* Tile::getBotRightTile()
{
    return this->getTileAtRelative(1, -1);
};

Tile* Tile::getBotMidTile()
{
    return this->getTileAtRelative(0, -1);
};

Tile* Tile::getBotLeftTile()
{
    return this->getTileAtRelative(-1, -1);
};

Tile* Tile::getMidLeftTile()
{
    return this->getTileAtRelative(-1, 0);
};

vector<Tile*>* Tile::getVacantAdjacentTiles()
{
    vector<Tile*>* adjacent_tiles = new vector<Tile*>;
    adjacent_tiles->push_back(getTopLeftTile());
    adjacent_tiles->push_back(getTopMidTile());
    adjacent_tiles->push_back(getTopRigtTile());
    adjacent_tiles->push_back(getMidRightTile());
    adjacent_tiles->push_back(getBotRightTile());
    adjacent_tiles->push_back(getBotMidTile());
    adjacent_tiles->push_back(getBotLeftTile());
    adjacent_tiles->push_back(getMidLeftTile());

    return adjacent_tiles;
};
