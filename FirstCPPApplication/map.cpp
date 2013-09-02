#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "map.h"
#include "game.h"
#include "tile.h"

#include "libtcod.hpp"

using namespace std;



Map::Map()
{

}

Map::~Map()
{

}

Tile * Map::getTileAt(int x, int y)
{
    return &(*tileVector)[y][x];
};

int Map::build_from_random(int seed)
{
    width = 10;
    height = 10;
    l_map = new TCODMap(width, height);
    //the default tile description
    description = "poppycock";

    // tileArray = new Tile[width*height];
    tileVector = new vector<vector<Tile>>;
    tileVector->resize(width);
    for(int ix = 0; ix < width; ++ix)
        (*tileVector)[ix].resize(height);

    int i = 0;
    int x = 0;
    int y = 0;

    while ( i < width*height )
    {
        (*tileVector)[y][x].map = this;
        (*tileVector)[y][x].updateTileType(3);
        if((*tileVector)[y][x].tiletype == 3)
        {
            //light passes though, walkable
            l_map -> setProperties(x, y, true, true);

            // printf("see through ");
            // printf("this should be true: %s\n", BoolToString(l_map->isWalkable(x, y)));
        }

        else 
        {
            //light does NOT pass through nor is walkable
            l_map -> setProperties(x, y, false, false);
            // printf("NOT see through ");
            // printf("this should be false: %s\n", BoolToString(l_map->isWalkable(x, y)));
        }

        // if(tileVector[y][x].tiletype == 2)
        // {
        //     WarpTileType* warp_tile;
        //     warp_tile = (WarpTileType*) tileVector[y][x].tile;

        //     getline (myfile,line);
        //     warp_tile->warpMap = atoi(line.c_str());
        //     getline (myfile,line);
        //     warp_tile->warpX = atoi(line.c_str());
        //     getline (myfile,line);
        //     warp_tile->warpY = atoi(line.c_str());

        //     getline (myfile,line);
        //     warp_tile->description = line;
        // }
        // else
        // {
            // getline (myfile,line);
            (*tileVector)[y][x].tile->description = "another desc";
            // tileVector[y][x].tile->description = line;
        // }

        (*tileVector)[y][x].tile_x = x;
        (*tileVector)[y][x].tile_y = y;

        // printf("x: %i, y: %i\n", x, y);
        if ( x >= (width -1)  ) // width is 1, only tile would be (0, 0) so you need to substract 1
        {
            y++;
            x = 0;
        }
        else 
        {
            x++;

        };

        i++;
    }

    return 1;

};

int Map::build_from_file(string filename)
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

        l_map = new TCODMap(width, height);
        // printf("Width: %s, Height: %s\n", width, height);
        // cout << width << " " << height << endl;

        // get default tile description
        getline (myfile,line);
        description = line;

        tileArray = new Tile[width*height];

        int i = 0;
        int x = 0;
        int y = 0;
        while (i < width*height) //had to change this from file.good because it was reading too far. Not sure yet.
        {
            getline (myfile,line);

            getline (myfile,line);
            int tileType = atoi(line.c_str());
            tileArray[i].map = this;
            tileArray[i].updateTileType(tileType);

            // printf("x %i y %i\n", x, y);
            if(tileArray[i].tiletype == 3)
            {
                //light passes though, walkable
                l_map -> setProperties(x, y, true, true);

                // printf("see through ");
                // printf("this should be true: %s\n", BoolToString(l_map->isWalkable(x, y)));
            }

            else 
            {
                l_map -> setProperties(x, y, false, false);
                // printf("NOT see through ");
                // printf("this should be false: %s\n", BoolToString(l_map->isWalkable(x, y)));
            }

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

            tileArray[i].tile_x = x;
            tileArray[i].tile_y = y;

            // printf("x: %i, y: %i\n", x, y);
            if ( x >= (width -1)  ) // width is 1, only tile would be (0, 0) so you need to substract 1
            {
                y++;
                x = 0;
            }
            else 
            {
                x++;

            };

            i++;
        }

        myfile.close();
    }

    return 1;
}

int Map::draw(Game *theGame)
{
    int i,j;

    for(j=0; j<width;j++)
    {
        for(i=0; i<height; i++)
        {
            Tile * the_tile = &(*tileVector)[i][j];

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
            };


            // printf("j %i i %i", j, i);
            if (l_map->isTransparent(j, i) == true)
            {
                // TCODConsole::root->putChar(j, i, 'w');
                // const TCODColor bg_color = TCODColor::amber;
                // TCODConsole::root->setCharBackground(j, i, bg_color, TCOD_BKGND_ADDALPHA(0.1));
                // printf("is\n" );
            }
            else
            {
                // printf("isn't\n" );
                // TCODConsole::root->putChar(j, i, 'n');
            };


        }
        TCODConsole::flush();
    }


    //may have just shot readability in the head here...
    // cout << endl << endl;
    // cout << "Tile Description:" << endl;

    Person  * thePerson = theGame->player;
    //BaseTileType * person_tile = tileArray[thePerson->x+(thePerson->y*width)].tile;
    BaseTileType * person_tile = (*tileVector)[thePerson->y][thePerson->x].tile;


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

    if(new_x >= width || new_x < 0 || new_y >= height || new_y < 0)
    {
        cout << "invalid move" << endl;
        return false;
    }

    Tile *player_tile; // the current player position
    //player_tile = &tileArray[thePerson->x+(thePerson->y*width)];
    player_tile = &(*tileVector)[thePerson->y][thePerson->x];

    Tile *target_tile; // the tile of the new position
    target_tile = &(*tileVector)[new_y][new_x];
    // target_tile = &tileArray[new_x+((new_y)*width)];
    //target_tile = &tileArray[new_x+((new_y)*width)];

    cout << "player tile x, y  ";
    cout << player_tile->tile_x << ", " ;
    cout <<player_tile->tile_y << endl;

    cout << "target tile x, y  ";
    cout << target_tile->tile_x << ", " ;
    cout << target_tile->tile_y << endl;

    std::stringstream converter;
    converter << target_tile->is_occupied();
    string bool_string = converter.str();

    cout << "Target tile is occupied: " << bool_string << endl;

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
