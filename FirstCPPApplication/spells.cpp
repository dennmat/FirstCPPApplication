#include "stdafx.h"

#include "spells.h"
#include "attribute_container.h"
#include "attribute.h"
#include "item_effect.h"



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
        this->attrs->health->current_val = 100;

        this->spell_effect = new ItemEffect;

        this->element = elements_t::FireElement;

};

TCODColor Spell::get_spell_color()
{
    std::unordered_map<elements_t, TCODColor> spell_color = std::unordered_map<elements_t, TCODColor>();

    spell_color.insert(std::make_pair<elements_t, TCODColor>(FireElement, TCODColor::red));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(WaterElement, TCODColor::blue));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(DeathElement, TCODColor::darkestGrey));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(LifeElement, TCODColor::lightGreen));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(SpectreElement, TCODColor::darkPurple));
    spell_color.insert(std::make_pair<elements_t, TCODColor>(CrystalElement, TCODColor::magenta));

    return spell_color.at(this->element);
};
