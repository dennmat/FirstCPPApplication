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

int Map::build(Person *thePerson)
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
			tileArray[i].representation='P';
			tileArray[i].type=1;
			tileArray[i].description="This is a warp tile. Type Wa[R]p to warp";
		}
		else
		{
			tileArray[i].collidable=false;
			tileArray[i].representation='#';
			tileArray[i].type=0;
			tileArray[i].description="You are standing in Empty Space";
		}
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
	cout << endl << endl << "Tile Description:" << endl << tileArray[thePerson->x+(thePerson->y*width)].description;
	cout << endl << endl;
	return 1;
}

bool Map::movePlayer(Person *thePerson, int x2, int y2)
{
	if(thePerson->x+x2 < width && thePerson->x+x2 > -1 &&
		thePerson->y+y2 < height && thePerson->y+y2 > -1 && tileArray[thePerson->x+x2+((thePerson->y+y2)*width)].collidable == false)
	{
		tileArray[thePerson->x+(thePerson->y*width)].representation = tempchar;
		tempchar = tileArray[thePerson->x+x2+((thePerson->y+y2)*width)].representation;
		tileArray[thePerson->x+x2+((thePerson->y+y2)*width)].representation = 'O';
		return true;
	}
	else
	{
		cout << endl << "invalid move" << endl;
		if(thePerson->x+x2 < width && thePerson->x+x2 > -1 && thePerson->y+y2 < height && thePerson->y+y2 > -1)
		{
			cout << tileArray[thePerson->x+x2+((thePerson->y+y2)*height)].description << endl;
		}
		return false;
	}
}