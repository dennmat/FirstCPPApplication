#include "stdafx.h"
#include <iostream>
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
	width = 10;
	height = 5;

	tileArray = new Tile[width*height];

	int i;
	for(i=0; i<width*height; i++)
	{
		if(i==4)
		{
			tileArray[i].collidable=true;
			tileArray[i].representation='X';
			tileArray[i].type=0;
			tileArray[i].description="A large bookshelf blocks your path.";
		}
		else if(i==6) 
		{
			tileArray[i].collidable=false;
			tileArray[i].representation='W';
			tileArray[i].type=1;
			tileArray[i].description="This is a warp tile. Type Wa[R]p to warp";
		}
		else
		{
			tileArray[i].collidable=false;
			tileArray[i].representation='.';
			tileArray[i].type=0;
			tileArray[i].description="You are standing in Empty Space";
		}
	}

	tempchar = tileArray[thePerson->x+(thePerson->y*width)].representation;
	tileArray[thePerson->x+(thePerson->y*width)].representation = thePerson->representation;

	tileArray[theEnemy->x+(theEnemy->y*width)].representation = theEnemy->representation;


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
	cout << endl << endl << "Tile Description:" << endl << tileArray[thePerson->x+(thePerson->y*width)].description;
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

        //make the player's tile collidable and make it look like him
        tempchar = target_tile->representation;
        target_tile->collidable = true;
        target_tile->representation = thePerson->representation;

        //the blank space for the command output
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
        {  // more blank space
            cout << endl;
        }
        return false;
    }
}
