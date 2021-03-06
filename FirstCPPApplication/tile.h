#ifndef TILE_H
#define TILE_H

#include <vector>

#include "object.h"
//#include "libtcod.hpp"
//#include "../FirstCPPApplication/libtcod_cpp_hpp/libtcod.hpp"
//#include "Representation.h"

class Map;
class Person;
class Actor;
class Object;
class Item;
class Inventory;

class Representation;
class DoorRepresentation;
class WarpRepresentation;
class FloorRepresentation;
class WallRepresentation;
class BaseRepresentation;

class TCODColor;


class Tile;

class BaseTileType //all tile types must inherit this
{
    public:
        bool collidable;
        Representation * representation;    // the character that represents this tile
        std::string description;      // the description of the tile
        int type_id;
        TCODColor* color;
        Tile* tile;

        BaseTileType() ;

};


class WarpTileType : public BaseTileType
{
    public:
        int warpMap;            // if this is a warp tile, index of the map to warp to
        int warpX, warpY;       // where you warp to in the destination map

        WarpTileType();
};


class WallTileType : public BaseTileType
{
    public: 
        WallTileType();
};


class DoorTileType : public BaseTileType
{
    public: 
        DoorTileType();
        bool is_open;

        void OpenDoor();
        void CloseDoor();
        void ToggleDoor();
};

class FloorTileType : public BaseTileType
{
    public: 
        FloorTileType();
};

class Tile : public Object
{
    bool _is_occupied;
    bool _is_known;

    public:
    BaseTileType * tile;
    Inventory* inventory;
    int type_id;               // type of tile

    int tile_x;
    int tile_y;

    Map* map; //the map that this tile is on

    std::vector<Actor *> * occupants;       // the dude sitting on the tile
    std::vector<Item *> * items;       // the dude sitting on the tile
    Actor * occupant;

    bool is_occupied() { return this->_is_occupied; };

    bool check_for_items();
    void place_item_down(Item* item);
    void pick_up_item(Item* item);

    bool is_known() { return this->_is_known; };
    void setKnown(bool is_known);

    void drawColorsToRoot(TCODColor* fg_color, TCODColor* bg_color);


    Tile(); 
    ~Tile(){ delete tile; };

    void updateTileType(int type = 0);
    void makeOccupied(Actor* the_actor);
    void makeUnoccupied(Actor* the_actor);

    Tile* getTileAtRelative(int x, int y);
    std::vector<Tile*>* Tile::getAdjacentTiles(int scale = 1);
    std::vector<Tile*>* getVacantAdjacentTiles();

    Tile* getTopLeftTile(int scale = 1);
    Tile* getTopMidTile(int scale = 1);
    Tile* getTopRightTile(int scale = 1);
    Tile* getMidRightTile(int scale = 1);
    Tile* getBotRightTile(int scale = 1);
    Tile* getBotMidTile(int scale = 1);
    Tile* getBotLeftTile(int scale = 1);
    Tile* getMidLeftTile(int scale = 1);
};

#endif
