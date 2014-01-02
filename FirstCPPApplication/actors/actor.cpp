#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>

#include "libtcod_cpp_hpp\libtcod.hpp"

#include "actor.h"

#include "thinker.h"
#include "inventory.h"
#include "equipment.h"
#include "tile.h"
#include "attribute.h"
#include "attribute_container.h"
#include "Representation.h"
#include "item.h"

using namespace std;

Actor::Actor()
{
    this->cls_name = "Actor";
    this->name = "Unset Actor name";

    this->representation = new Representation;
    this->inventory = new Inventory;
    this->attrs = new AttributeContainer;
    this->attrs->owner = this;

    this->actors_in_sight = new std::vector<Actor*>;

    this->thinker = new Thinker;
    this->thinker->master = this;

    this->is_active = true;

    this->is_moving_left = false;
    this->is_moving_right = false;
    this->is_moving_up = false;
    this->is_moving_down = false;

    this->l_path = NULL;
    this->has_attacked = false;
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

bool Actor::has_attributes()
{
    return this->attrs != NULL;
};

Item* Actor::CreateCorpse()
{
    Item* corpse = new Item;
    corpse->repr->repr = '%';
    corpse->repr->setFGColor(*this->representation->fg_color, true, false, true);
    
    corpse->name =  this->cls_name + "corpse";


    return corpse;

};

void Actor::Die()
{
    //make the master's tile no longer occupied by him
    //drop corpse on floor
    Item* corpse = this->CreateCorpse();
    this->my_tile->place_item_down(corpse);

    //remove master from ai update list
    this->is_active = false;
    this->putPerson(NULL, NULL, NULL);
};
