#include "stdafx.h"
#include <vector>
#include <algorithm>

#include "item_effect.h"
#include "attribute_container.h"
#include "attribute.h"
#include "actor.h"

ItemEffect::ItemEffect()
{
    this->health_current_val = 9999;
    this->health_max_val = 0;
    this->health_regen_rate = 0;
    this->health_regen_interval = 0; 

    this->mana_current_val = 0;
    this->mana_max_val = 0;
    this->mana_regen_rate = 0;
    this->mana_regen_interval = 0; 

    this->armor_current_val = 0;
    this->armor_max_val = 0;
    this->armor_regen_rate = 0;
    this->armor_regen_interval = 0; 

    this->damage_current_val = 0;
    this->damage_max_val = 0;
    this->damage_regen_rate = 0;
    this->damage_regen_interval = 0; 

    this->applied_to = new std::vector<Actor*>;
};

bool ItemEffect::already_applied(Actor* actor)
{

    //if actor in list of people this item's already applied to, no reason to do
    //it again
    auto it = std::find(this->applied_to->begin(), this->applied_to->end(), actor);

    return  it == this->applied_to->end();

    // The thing is that I need to sort out how to apply the item's effects to
    // the actor once, because if it gets reapplied they might be invincible or
    // gaining way too much health for ever.
    //TODO TODO TODO


};

void ItemEffect::ApplyHealthEffects(Actor* actor)
{
    actor->attrs->health->AddToCurrentVal(this->health_current_val);
    actor->attrs->health->AddToMaxVal(this->health_max_val);
    actor->attrs->health->AddToRegenRate(this->health_regen_rate);
    actor->attrs->health->AddToRegenInterval(this->health_regen_interval);
};

void ItemEffect::ApplyManaEffects(Actor* actor)
{
    actor->attrs->mana->AddToCurrentVal(this->mana_current_val);
    actor->attrs->mana->AddToMaxVal(this->mana_max_val);
    actor->attrs->mana->AddToRegenRate(this->mana_regen_rate);
    actor->attrs->mana->AddToRegenInterval(this->mana_regen_interval);
};

void ItemEffect::ApplyArmorEffects(Actor* actor)
{
    actor->attrs->armor->AddToCurrentVal(this->armor_current_val);
    actor->attrs->armor->AddToMaxVal(this->armor_max_val);
    actor->attrs->armor->AddToRegenRate(this->armor_regen_rate);
    actor->attrs->armor->AddToRegenInterval(this->armor_regen_interval);
};

void ItemEffect::ApplyDamageEffects(Actor* actor)
{
    actor->attrs->damage->AddToCurrentVal(this->damage_current_val);
    actor->attrs->damage->AddToMaxVal(this->damage_max_val);
    actor->attrs->damage->AddToRegenRate(this->damage_regen_rate);
    actor->attrs->damage->AddToRegenInterval(this->damage_regen_interval);

};

void ItemEffect::RemoveHealthEffects(Actor* actor)
{
    actor->attrs->health->RemoveFromCurrentVal(this->health_current_val);
    actor->attrs->health->RemoveFromMaxVal(this->health_max_val);
    actor->attrs->health->RemoveFromRegenRate(this->health_regen_rate);
    actor->attrs->health->RemoveFromRegenInterval(this->health_regen_interval);
};

void ItemEffect::RemoveManaEffects(Actor* actor)
{
    actor->attrs->mana->RemoveFromCurrentVal(this->mana_current_val);
    actor->attrs->mana->RemoveFromMaxVal(this->mana_max_val);
    actor->attrs->mana->RemoveFromRegenRate(this->mana_regen_rate);
    actor->attrs->mana->RemoveFromRegenInterval(this->mana_regen_interval);
};

void ItemEffect::RemoveArmorEffects(Actor* actor)
{
    actor->attrs->armor->RemoveFromCurrentVal(this->armor_current_val);
    actor->attrs->armor->RemoveFromMaxVal(this->armor_max_val);
    actor->attrs->armor->RemoveFromRegenRate(this->armor_regen_rate);
    actor->attrs->armor->RemoveFromRegenInterval(this->armor_regen_interval);
};

void ItemEffect::RemoveDamageEffects(Actor* actor)
{
    actor->attrs->damage->RemoveFromCurrentVal(this->damage_current_val);
    actor->attrs->damage->RemoveFromMaxVal(this->damage_max_val);
    actor->attrs->damage->RemoveFromRegenRate(this->damage_regen_rate);
    actor->attrs->damage->RemoveFromRegenInterval(this->damage_regen_interval);
};



