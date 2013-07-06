#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "world.h"
#include "game.h"

using namespace std;

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
		while ( myfile.good() )
		{
			getline (myfile,line);
			test = (char*)line.c_str();
			tileArray[i].representation = test[0];
													
			getline (myfile,line);
			tileArray[i].tiletype = atoi(line.c_str());

			if(tileArray[i].tiletype == 2)
			{
				getline (myfile,line);
				tileArray[i].warpMap = atoi(line.c_str());
				getline (myfile,line);
				tileArray[i].warpX = atoi(line.c_str());
				getline (myfile,line);
				tileArray[i].warpY = atoi(line.c_str());

				getline (myfile,line);
				tileArray[i].description = line;
			}
			else
			{
				getline (myfile,line);
				tileArray[i].description = line;
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

    Person * thePerson = theGame->player;

	for(i=0; i<height; i++)
	{
		cout << endl;
		for(j=0; j<width;j++)
		{
			if(j==thePerson->x && i == thePerson->y)
				cout << '@';
			else
				cout << tileArray[(i*width)+j].representation;
		}
	}

	if(tileArray[thePerson->x+(thePerson->y*width)].description != "none")
	{
		cout << endl << endl << "Tile Description:" << endl << tileArray[thePerson->x+(thePerson->y*width)].description;
	}
	else
		cout << endl << endl << "Tile Description:" << endl << description;

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
            target_tile->tiletype == 0 || target_tile->tiletype == 2)
    {
		cout << endl << endl << endl;
        return true;
    }

    else
    {
        cout << endl << "invalid move" << endl;
        if(new_x < width && new_x > -1 && new_y < height && new_y > -1)
        {
            cout << target_tile->description << endl;
        }
        else
        {  // more blank space for gui consistency
            cout << endl;
        }
        return false;
    }
}
