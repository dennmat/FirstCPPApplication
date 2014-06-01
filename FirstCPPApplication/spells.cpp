#include "stdafx.h"

#include "spells.h"
#include "attribute_container.h"
#include "attribute.h"
#include "item_effect.h"
#include "tile.h"



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

    this->aoe = 0;
    this->target_type = TargetTypes::TargettedTargetType;

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

void Spell::cast_spell(Actor* target)
{
    this->spell_effect->ApplyAllEffects(target);

    if (this->spell_effect->duration > 0)
    {
        TimedEffect* timed_effect = new TimedEffect;
        timed_effect->effect = this->spell_effect;
        timed_effect->turn_applied = Game::turn_count;
        target->timed_spell_effects->push_back(timed_effect);
    };

};


std::vector<Actor*> Spell::targets_for_tile(Tile* target_tile)
{
    std::vector<Tile*>* adjacent_tiles = target_tile->getAdjacentTiles(1);
    typedef std::vector<Tile*> tile_vector;
    std::vector<Actor*> targets;
    for (tile_vector::iterator it = adjacent_tiles->begin(); it != adjacent_tiles->end(); it++)
    {
        if ((*it)->is_occupied())
        {
            targets.push_back((*it)->occupant);
        }
    };
    delete adjacent_tiles;

    return targets;

};
