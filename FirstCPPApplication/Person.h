#ifndef PERSON_H
#define PERSON_H
#pragma once
#include <sstream>



class Tile;
class Person{
    public:
    std::string name;
    int age;
	int x,y;

    char representation;
    void putPerson(Tile * current_tile, Tile * next_tile, int new_x, int new_y);
};
#endif
