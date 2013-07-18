#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "world.h"
#include "game.h"

#include "libtcod.hpp"

using namespace std;

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


Map::Map()
{

}

Map::~Map()
{

}

int Map::build(string filename)
{
    string line;
    ifstream myfile (filename);

    if (myfile.is_open())
    {
        // get width
        getline (myfile,line);
        width = atoi(line.c_str());

        // get height
        getline (myfile,line);
        height = atoi(line.c_str());

        // get default tile description
        getline (myfile,line);
        description = line;

        tileArray = new Tile[width*height];

        int i=0;
        char *test;
        while (i < width*height) //had to change this from file.good because it was reading too far. Not sure yet.
        {
            getline (myfile,line);
            test = (char*)line.c_str();
            // tileArray[i].tile->representation = test[0];

            getline (myfile,line);
            int tileType = atoi(line.c_str());
            // tileArray[i].tiletype = tileType;
            tileArray[i].updateTileType(tileType);

            if(tileArray[i].tiletype == 2)
            {
                WarpTileType* warp_tile;
                warp_tile = (WarpTileType*) tileArray[i].tile;

                getline (myfile,line);
                warp_tile->warpMap = atoi(line.c_str());
                getline (myfile,line);
                warp_tile->warpX = atoi(line.c_str());
                getline (myfile,line);
                warp_tile->warpY = atoi(line.c_str());

                getline (myfile,line);
                warp_tile->description = line;
            }
            else
            {
                getline (myfile,line);
                tileArray[i].tile->description = line;
            }

            i++;
        }

        myfile.close();
    }

    return 1;
}

int Map::draw(Game *theGame)
{
    int i,j;

    for(i=0; i<height; i++)
    {
        // cout << endl;
        for(j=0; j<width;j++)
        {
            Tile * the_tile = &tileArray[(i*width)+j];
            if(the_tile->is_occupied())
            {
                // cout << the_tile->occupant->name << endl;
		char the_char = the_tile->occupant->representation->repr;
		TCODColor the_color = the_tile->occupant->representation->color;
                TCODConsole::root->putChar(j, i, the_char);
                TCODConsole::root->setCharForeground(j , i, the_color);
                // cout << the_tile->occupant->representation;
            }
            else
            {
		char the_char = the_tile->tile->representation->repr;
		TCODColor the_color = the_tile->tile->representation->color;
                //TCODColor color = the_tile->tile->color;

                TCODConsole::root->putChar(j, i, the_char );
                TCODConsole::root->setCharForeground(j , i, the_color);
                // cout << the_tile->tile->representation;
            }
        }
        TCODConsole::flush();
    }


    //may have just shot readability in the head here...
    // cout << endl << endl;
    // cout << "Tile Description:" << endl;

    Person  * thePerson = &theGame->player;
    BaseTileType * person_tile = tileArray[thePerson->x+(thePerson->y*width)].tile;

    string pers_desc = person_tile->description;
    string tile_description = (pers_desc != "none" ?  pers_desc : description);
    // cout << tile_description;
    // cout << endl << endl;

    return 1;
}

bool Map::movePlayer(Person *thePerson, int x2, int y2)
{
    // cout << "trying to move player" << endl;

    int new_x, new_y; // where the player is intending to go
    new_x = thePerson->x+x2;
    new_y = thePerson->y+y2;

    Tile *player_tile; // the current player position
    player_tile = &tileArray[thePerson->x+(thePerson->y*width)];

    Tile *target_tile; // the tile of the new position
    target_tile = &tileArray[new_x+((new_y)*width)];

    if(new_x < width && new_x > -1 &&
            new_y < height && new_y > -1 &&
            (target_tile->tiletype == 3 || target_tile->tiletype == 2) &&
            !target_tile->is_occupied())
    {
        // cout << endl << endl << endl;
        thePerson->putPerson(target_tile, new_x, new_y);
        return true;
    }

    else if (target_tile->is_occupied())
    {
        bool is_fighter = target_tile->occupant->is_fighter;
        if (is_fighter)
        {
            thePerson->attack(target_tile->occupant);
            return false;
        }
    }

    else
    {
        cout << endl << "invalid move" << endl;
        if(new_x < width && new_x > -1 && new_y < height && new_y > -1)
        {
            cout << target_tile->tile->description << endl;
        }
        else
        {  // more blank space for gui consistency
            cout << endl;
        }

        return false;
    }
}
