#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

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
#include "item_effect.h"


Actor::Actor()
{
    this->cls_name = "Actor";
    this->name = "Unset Actor name";

    this->xp = 0;
    this->xp_this_level = 0;
    this->xp_required = 100;
    this->xp_value = 10;
    this->level = 1;

    this->representation = new Representation;
    this->inventory = new Inventory;
    this->inventory->master = this;
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

    std::vector<Actor*>::iterator aisItr;
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
        std::cout << "next tile is null" << std::endl;
    }
    else
    {
        std::cout << "else, this shouldn't be a possiblity" << std::endl;
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
    
    corpse->name =  this->cls_name + " corpse";
    corpse->item_effect->set_all_vals_to(0);
    corpse->item_effect->health_regen_interval=1;
    corpse->item_effect->health_regen_rate=1;
    corpse->usable = true;

    return corpse;
};

int* Actor::get_direction_heading()
{
    int dir_array[2];

    //handle x coord
    if (this->is_moving_left)
    {
        dir_array[0] = -1;
    }
    else if (this->is_moving_right)
    {
        dir_array[0] =  1;
    }
    else 
    {
        dir_array[0] = 0;
    };

    //handle y coord
    if (this->is_moving_up)
    {
        dir_array[1] = -1;
    }
    else if (this->is_moving_down)
    {
        dir_array[1] =  1;
    }
    else 
    {
        dir_array[1] = 0;
    };

    return dir_array;

};

void Actor::Die()
{
    //make the master's tile no longer occupied by him
    //drop corpse on floor or another item
    TCODRandom *rng = TCODRandom::getInstance();
    Item* dropped_item;
    int result = rng->getInt(0, 100);
    if (result % 2 == 0)
    {
        dropped_item = this->CreateCorpse();
    }
    else if (result % 3 == 0)
    {
        dropped_item = new Item;
        dropped_item->repr->repr = '&';
        dropped_item->repr->setFGColor(TCODColor::grey, true, false, true);
        dropped_item->name = "Chainmail";
        dropped_item->item_effect->set_all_vals_to(0);
        dropped_item->slot_type = slots_t::Chest;

        //sword damage
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int armor = rng->getInt(1, 5, 2);
        dropped_item->item_effect->armor_current_val = armor;
        dropped_item->item_effect->armor_max_val = armor;
    }
    else 
    {
        dropped_item = new Item;
        dropped_item->repr->repr = '/';
        dropped_item->repr->setFGColor(TCODColor::grey, true, false, true);
        dropped_item->name = "A sword";
        dropped_item->item_effect->set_all_vals_to(0);
        dropped_item->slot_type = slots_t::MainHand;

        //sword damage
        rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
        int damage = rng->getInt(5, 15, 10);
        dropped_item->item_effect->damage_current_val = damage;
        dropped_item->item_effect->damage_max_val = damage;
    }

    this->my_tile->place_item_down(dropped_item);

    //remove master from ai update list
    this->is_active = false;
    this->putPerson(NULL, NULL, NULL);

    //destroy ai
    if (this->thinker != NULL)
    {
        delete this->thinker;
    };

};

