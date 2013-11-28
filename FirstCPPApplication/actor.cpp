#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>

#include "actor.h"
#include "thinker.h"
#include "inventory.h"

using namespace std;

Actor::Actor()
{
    l_path = NULL;
    representation = new Representation;
    inventory = new Inventory;
    name = "Unset Actor name";

    actors_in_sight = new std::vector<Actor*>;

    thinker = new Thinker;
    thinker->master = this;

    is_moving_left = false;
    is_moving_right = false;
    is_moving_up = false;
    is_moving_down = false;

    has_attacked = false;
};

void Actor::resetIsMoving()
{
    this->is_moving_up = false; 
    this->is_moving_right = false;
    this->is_moving_down = false;
    this->is_moving_left = false;
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
    if (aisItr != this->actors_in_sight->end()) { return true; }
    else { return false; }
};

void Actor::ActorInSight(int x, int y, Actor * actor)
{
    std::vector<Actor*>* ais = this->actors_in_sight;
    //if the actor is in sight and isnt this actor:
    if (actor == this) {return;}
    if(std::find(ais->begin(), ais->end(), actor) != ais->end()) { }
    else { this->actors_in_sight->push_back(actor);
    }

}

void Actor::putPerson( Tile * next_tile, int new_x, int new_y)
{	//puts a person on a tile, resets the old tile

    if (my_tile != NULL){
        my_tile->makeUnoccupied(this); 
    }

    if (next_tile != NULL){
        this->my_tile = next_tile;
        next_tile->makeOccupied(this); 
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

void Actor::pickUpItem(Item* item)
{

    //add to inventory
    this->inventory->add_item(item);

    // remove from Tiles
    this->my_tile->pick_up_item(item);

};
