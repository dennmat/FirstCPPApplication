#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "world.h"

using namespace std;

Map::Map()
{

}

Map::~Map()
{
    free(tileArray);
}

int Map::build(Person *thePerson, Person *theEnemy)
{
	string line;
	ifstream myfile ("office.txt");
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
		description = new char[255];
		strcpy(description, line.c_str());
		

		tileArray = new Tile[width*height];

		int i=0;
		char *test;
		while ( myfile.good() )
		{
			getline (myfile,line);
			test = (char*)line.c_str();
			tileArray[i].representation = test[0];
													
			getline (myfile,line);
			tileArray[i].collidable = atoi(line.c_str());
			getline (myfile,line);

			tileArray[i].description = new char[line.length()];
			strcpy(tileArray[i].description, line.c_str());
			i++;
		}
		myfile.close();
	}

	tempchar = tileArray[thePerson->x+(thePerson->y*width)].representation;
	tileArray[thePerson->x+(thePerson->y*width)].representation = 'O';

	return 1;
}

int Map::draw(Person *thePerson)
{
	int i,j;

	for(i=0; i<height; i++)
	{
		cout << endl;
		for(j=0; j<width;j++)
		{
			cout << tileArray[(i*width)+j].representation;
		}
	}

	if(strcmp(tileArray[thePerson->x+(thePerson->y*width)].description, "none") == 1)
		cout << endl << endl << "Tile Description:" << endl << tileArray[thePerson->x+(thePerson->y*width)].description;
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
            target_tile->collidable == false)
    {
        //reset tile to what it was before
        player_tile->representation = tempchar;
        player_tile->collidable = false;
        player_tile->occupant = 0;


        //make the player's tile collidable and make it look like him
        tempchar = target_tile->representation;
        target_tile->collidable = true;
        target_tile->representation = thePerson->representation;
        target_tile->occupant = thePerson;

        //the blank space for the command output
        cout << endl << endl << endl;

        return true;
    }
    // else if (target_tile->fighter == true)
    // {
    //     //there's someone here we want to fight

    // }
    else
    {
        cout << endl << "invalid move" << endl;
        if(new_x < width && new_x > -1 && new_y < height && new_y > -1)
        {
            cout << target_tile->description << endl;
        }
        else
        {  // more blank space
            cout << endl;
        }
        return false;
    }
}
