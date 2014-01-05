
#include "stdafx.h"
#include "item.h"
#include "Representation.h"
#include "item_effect.h"
#include "actors/actor.h"

Item::Item()
{
    this->repr = new BaseItemRepresentation();
    this->slot_type =  slots_t::Chest;
    this->item_effect = new ItemEffect;

    this->name = "A Nameless Item";
    this->description = "A Descriptionless Item";
};


void Item::use(Actor* target)
{
    this->item_effect->ApplyAllEffects(target);
};
