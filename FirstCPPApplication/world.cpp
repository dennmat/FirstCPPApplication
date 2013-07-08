#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "world.h"
#include "game.h"

using namespace std;

Tile::Tile()
{
    tiletype = 0;
    is_occupied = false;

    updateTileType(tiletype);
};

void Tile::makeOccupied()
{
    is_occupied = true;

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

    // this set of 4 LOC should be handled in a moveActor type of function where their old tile
    // is made unoccupied, and the new one is, rather than manually setting it every time
    //
    // that'll have the option to make the whole movement of AI more convienient since we won't
    // be worrying about the occupany all the time
    Person  thePerson = theGame->player;

    BaseTileType * person_tile = tileArray[thePerson.x+(thePerson.y*width)].tile;
    tileArray[thePerson.x+(thePerson.y*width)].is_occupied = true;
    tileArray[thePerson.x+(thePerson.y*width)].occupant = &thePerson;

    for(i=0; i<height; i++)
    {
        cout << endl;
        for(j=0; j<width;j++)
        {
            Tile * the_tile = &tileArray[(i*width)+j];
            if(the_tile->is_occupied)
            {
                cout << the_tile->occupant->representation;
            }
            else
            {
                cout << the_tile->tile->representation;
            }
        }
    }
    tileArray[thePerson.x+(thePerson.y*width)].is_occupied = false;


    //may have just shot readability in the head here...
    cout << endl << endl;
    cout << "Tile Description:" << endl;

    string tile_description = (person_tile->description != "none" ?  person_tile->description : description);
    cout << tile_description;
    cout << endl << endl;

    return 1;
}

bool Map::movePlayer(Person *thePerson, int x2, int y2)
{
    int new_x, new_y; // where the player is intending to go
    new_x = thePerson->x+x2;
    new_y = thePerson->y+y2;

    Tile *target_tile; // the tile of the new position
    target_tile = &tileArray[new_x+((new_y)*width)];

    Tile *player_tile; // the current player position
    player_tile = &tileArray[thePerson->x+(thePerson->y*width)];

    if(new_x < width && new_x > -1 &&
            new_y < height && new_y > -1 &&
            target_tile->tiletype == 3 || target_tile->tiletype == 2)
    {
        cout << endl << endl << endl;
        return true;
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
