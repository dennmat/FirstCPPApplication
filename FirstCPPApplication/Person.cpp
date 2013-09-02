#include "stdafx.h"
#include "tile.h"
#include "map.h"
#include "Person.h"

#include <iostream>
#include <sstream>
// #include <fstream>

Person::Person(string name, int age, int x, int y, char repr, string pet_name){
    this->name = name;
    this->age = age;
    this->x = x;
    this->y = y;

    //set its representation, aka color and char
    Representation * new_repr =   new Representation;
    this->representation = new_repr;
    this->representation->repr = repr;

    is_fighter = true;

    pet = new Pet;
    pet->assign_to_master(this);
    has_live_pet = true;

    my_tile = NULL;

};

Person::Person()
{
    this->name = "Unnamed";
    this->age = 999;
    this->x = 10;
    this->y = 10;

    this->dest_x = -1;
    this->dest_y = -1;

    //set its representation, aka color and char
    Representation * new_repr =   new Representation;
    this->representation = new_repr;
    this->representation->repr = '~';

    is_fighter = true;

    pet = new Pet;
    pet->assign_to_master(this);
    has_live_pet = true;

    my_tile = NULL;
    l_path = NULL;
};

Person::~Person(){
    //TODO: safely remove pet
};

void Person::update()
{

    //PATH FINDING
    Map* map = my_tile->map;
    if (l_path == NULL)
    {
        l_path = new TCODPath(map->l_map);

        TCODRandom * rnd = TCODRandom::getInstance();
        dest_x = rnd->getInt(1, map->width-1);
        dest_y = rnd->getInt(1, map->height-1);
        l_path->compute(x, y, dest_x, dest_y);
    }
    else
    {
        // cout << "Path size: " << l_path->size() << endl;
        // l_path->walk(&x, &y, false);
        // Tile * next_tile = map->getTileAt(x,y);
        // putPerson(next_tile, x, y);https://www.youtube.com/watch?feature=player_embedded&v=8wWuH7MIeCA&noredirect=1
    };

    //PET UPDATE
    if (has_live_pet == true)
    {
        pet->update();
    }
    else
    {
        printf("no pet\n");
    }

};

void Person::putPerson( Tile * next_tile, int new_x, int new_y)
{	//puts a person on a tile, resets the old tile

    if (my_tile != NULL){
        // my_tile->occupant = NULL;
        my_tile->makeUnoccupied(this); 
    }

    if (next_tile != NULL){
        //next_tile->occupant = this;
        //next_tile->occupants->push_back(this);
        this->my_tile = next_tile;
        next_tile->makeOccupied(this); 
        // cout << "new occupant: " << name << endl;
    }
    else if (next_tile == NULL)
    {
        cout << "next tile is null" << endl;
    }
    else
    {
        cout << "else, this shouldn't be a possiblity" << endl;
    }

    if (new_x != -1 && new_y != -1)
    {
        x = new_x;
        y = new_y;
    }

};


void Person::attack(Actor * target)
{
    pet->Attack(((Person*)target)->pet, 10); //shit will happen if the target isn't a Person
};
