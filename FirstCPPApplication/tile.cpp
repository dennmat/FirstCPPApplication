#include "stdafx.h"
#include "tile.h"
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "libtcod_cpp_hpp\libtcod.hpp"

#include "tile.h"
#include "map.h"
#include "item.h"
#include "inventory.h"
#include "Representation.h"
#include "utils.h"



using namespace std;

Tile::Tile()
{
    type_id = 0;
    _is_occupied = false;
    _is_known = false;

    occupants = new std::vector<Actor*>;
    // items = new std::vector<Item*>;
    inventory = new Inventory;

    updateTileType(type_id);
};

bool Tile::check_for_items() { 
    bool are_items = this->inventory->get_count() != 0; 
    return are_items;
};

void Tile::drawColorsToRoot(TCODColor* fg_color, TCODColor* bg_color)
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
};

void Tile::place_item_down(Item* item)
{
    this->inventory->add_item(item);
};
void Tile::pick_up_item(Item* item)
{
    this->inventory->remove_item(item);
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
    this->type_id = type;

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
    this->tile->tile=this;

};

Tile* Tile::getTileAtRelative(int x, int y)
{
    //if tile is in map get tile, otherwise fail nicely
    int new_x = this->tile_x + x;
    int new_y = this->tile_y + y;

    //if (new_x <= this->map->width && new_x >= 0 
    //        && new_y <= this->map->height && new_y >= 0)
    //{
        return this->map->getTileAt(new_x, new_y);
    //}
    //else 
    //{
    //    return NULL;
    //};
};

Tile* Tile::getTopLeftTile(int scale)
{
    return this->getTileAtRelative(-1*scale, -1*scale);
};

Tile* Tile::getTopMidTile(int scale)
{
    return this->getTileAtRelative(0*scale, -1*scale);
};

Tile* Tile::getTopRightTile(int scale)
{
    return this->getTileAtRelative(1*scale, -1*scale);
};

Tile* Tile::getMidRightTile(int scale)
{
    return this->getTileAtRelative(1*scale, 0*scale);
};

Tile* Tile::getBotRightTile(int scale)
{
    return this->getTileAtRelative(1*scale, 1*scale);
};

Tile* Tile::getBotMidTile(int scale)
{
    return this->getTileAtRelative(0*scale, 1*scale);
};

Tile* Tile::getBotLeftTile(int scale)
{
    return this->getTileAtRelative(-1*scale, 1*scale);
};

Tile* Tile::getMidLeftTile(int scale)
{
    return this->getTileAtRelative(-1*scale, 0*scale);
};

vector<Tile*>* Tile::getAdjacentTiles(int scale)
{
    vector<Tile*>* adjacent_tiles = new vector<Tile*>;
    adjacent_tiles->push_back(getTopLeftTile(scale));
    adjacent_tiles->push_back(getTopMidTile(scale));
    adjacent_tiles->push_back(getTopRightTile(scale));
    adjacent_tiles->push_back(getMidRightTile(scale));
    adjacent_tiles->push_back(getBotRightTile(scale));
    adjacent_tiles->push_back(getBotMidTile(scale));
    adjacent_tiles->push_back(getBotLeftTile(scale));
    adjacent_tiles->push_back(getMidLeftTile(scale));

    return adjacent_tiles;

};

vector<Tile*>* Tile::getVacantAdjacentTiles()
{
    vector<Tile*>* adjacent_tiles = this->getAdjacentTiles();
    //remove all adjacent null tile ptrs
    adjacent_tiles->erase(
            std::remove_if(
                adjacent_tiles->begin(),
                adjacent_tiles->end(),
                [](Tile* s) -> bool { return s == NULL; }),
            adjacent_tiles->end());

    return adjacent_tiles;
};

BaseTileType::BaseTileType() 
{
    this->description = "Another descriptionless tile";
    type_id = 0;
    representation = new BaseRepresentation; 
};

WarpTileType::WarpTileType() : BaseTileType() 
{
    this->description = "A strange color eminates from the ground";
    type_id = 2;
    representation = new WarpRepresentation; 
};

WallTileType::WallTileType() : BaseTileType() 
{
    this->description = "Non descript wall";
    type_id = 1;
    representation = new WallRepresentation;
};

DoorTileType::DoorTileType()  : BaseTileType() 
{
    this->description = "A door, it's probably open, but who can even tell?";
    this->is_open = false;
    type_id = 4;
    representation = new DoorRepresentation; 
};

void DoorTileType::ToggleDoor()
{
    if (this->is_open)
    {
        this->CloseDoor();
    }
    else
    {
        this->OpenDoor();
    };
}

void DoorTileType::CloseDoor()
{
    this->description = "A door is closed here";
    this->is_open = false;
    this->representation->repr = '+';
    this->tile->map->l_map->setProperties(this->tile->tile_x, this->tile->tile_y, false, false);
};

void DoorTileType::OpenDoor()
{
    this->description = "An open door";
    this->is_open = true;
    this->representation->repr = '=';
    this->tile->map->l_map->setProperties(this->tile->tile_x, this->tile->tile_y, true, true);
};


FloorTileType::FloorTileType() : BaseTileType() 
{
    this->description = "Plain old floor";
    type_id = 3;
    representation = new FloorRepresentation; 
};
