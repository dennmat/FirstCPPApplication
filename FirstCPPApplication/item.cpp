
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
    this->description = "A Descriptionless Item.";

    this->usable = false;
    this->equippable = false;
};


void Item::use(Actor* target)
{
    if ( this->usable )
    {
        this->item_effect->ApplyAllEffects(target);
    }
    else
    {
        std::cout << "Not a usable item, try equipping it" << std::endl;
    };
};

void Item::unequip(Actor* target)
{
    if ( this->equippable )
    {
        this->item_effect->RemoveAllEffects(target);
    }
    else
    {
        std::cout << "Not a unequippable item" << std::endl;
    };
};

void Item::equip(Actor* target)
{
    if ( this->equippable )
    {
        this->item_effect->ApplyAllEffects(target);
    }
    else
    {
        std::cout << "Not a equippable item, try using it" << std::endl;
    };
};
