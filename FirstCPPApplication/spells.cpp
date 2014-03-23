#include "stdafx.h"

#include "spells.h"
#include "attribute_container.h"
#include "attribute.h"

Spell::Spell()
{
        this->name = "Unamed spell";
        this->mana_cost = 10;

        this->turn_cooldown = 1;
        this->max_range = 7;
        this->min_range = 1;
        this->min_char_level = 1;
        this->cast_count = 0;

        this->attrs = new AttributeContainer;
        this->attrs->health->current_val = -10;

};
