
#include "stdafx.h"
#include "item.h"
#include "game.h"
#include "Representation.h"
#include "item_effect.h"
#include "actors/actor.h"
#include "tile.h"
#include "inventory.h"

Item::Item()
{
    this->repr = new BaseItemRepresentation();
    this->slot_type =  slots_t::Chest;
    this->item_effect = new ItemEffect;
    this->weight = 1;

    this->name = "A Nameless Item";
    this->description = "A Descriptionless Item.";

    this->usable = false;
    this->uses = 1;
    this->equippable = false;
};


void Item::use(Actor* target)
{
    if ( this->usable )
    {
        this->item_effect->ApplyAllEffects(target);
        this->uses--;
        if (this->item_effect->is_timed())
        {
          TimedEffect* timed_effect = new TimedEffect;
            timed_effect->effect = this->item_effect;
            timed_effect->turn_applied = Game::turn_count;
            target->timed_item_effects->push_back(timed_effect);
        };
        if (this->uses <= 0)
        {
            target->inventory->remove_item(this);
            // delete this; //dont want to delete it if its a timed item
        }
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
        //this doesn't happen here any more
        // this->item_effect->ApplyAllEffects(target);
    }
    else
    {
        std::cout << "Not a equippable item, try using it" << std::endl;
    };
};
