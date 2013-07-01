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

int Map::build(Person *thePerson)
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
	if(thePerson->x+x2 < width && thePerson->x+x2 > -1 &&
		thePerson->y+y2 < height && thePerson->y+y2 > -1 && tileArray[thePerson->x+x2+((thePerson->y+y2)*width)].collidable == false)
	{
		tileArray[thePerson->x+(thePerson->y*width)].representation = tempchar;
		tempchar = tileArray[thePerson->x+x2+((thePerson->y+y2)*width)].representation;
		tileArray[thePerson->x+x2+((thePerson->y+y2)*width)].representation = 'O';

        //the blank space for the command output
        cout << endl << endl << endl;

		return true;
	}
	else
	{
		cout << endl << "invalid move" << endl;
		if(thePerson->x+x2 < width && thePerson->x+x2 > -1 && thePerson->y+y2 < height && thePerson->y+y2 > -1)
		{
			cout << tileArray[thePerson->x+x2+((thePerson->y+y2)*height)].description << endl;
		}
        else
        {  // more blank space
            cout << endl;
        }
		return false;
	}
}
