#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>

#include "actor.h"
#include "thinker.h"

using namespace std;

Actor::Actor()
{
    l_path = NULL;
    representation = new Representation;
    name = "Unset Actor name";

    actors_in_sight = new std::vector<Actor*>;

    thinker = new Thinker;
    thinker->master = this;

    is_moving_left = false;
    is_moving_right = false;
    is_moving_up = false;
    is_moving_down = false;
};

std::string Actor::GetName()
{
    return name;
};

const char* Actor::GetNameC()
{
    return name.c_str();
};

bool Actor::IsActorInSight(Actor * actor)
{

    vector<Actor*>::iterator aisItr;
    aisItr = std::find(this->actors_in_sight->begin(), this->actors_in_sight->end(),  actor);
    if (aisItr != this->actors_in_sight->end())
    { return true; }
    else 
    { return false; }
};

void Actor::ActorInSight(int x, int y, Actor * actor)
{
    std::vector<Actor*>* ais = this->actors_in_sight;
    //if the actor is in sight and isnt this actor:
    if (actor == this) {return;}
    if(std::find(ais->begin(), ais->end(), actor) != ais->end()) 
    {
    }
    else
    {
        this->actors_in_sight->push_back(actor);
    }

}

void Actor::putPerson( Tile * next_tile, int new_x, int new_y)
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
