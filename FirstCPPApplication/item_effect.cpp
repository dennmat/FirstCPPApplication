#include "stdafx.h"
#include <vector>
#include <algorithm>

#include "item_effect.h"
#include "attribute_container.h"
#include "attribute.h"
#include <actors/actor.h>
#include "utils.h"

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

void ItemEffect::set_all_vals_to(int new_val)
{
    this->health_current_val = new_val;
    this->health_max_val = new_val;
    this->health_regen_rate = new_val;
    this->health_regen_interval = new_val; 

    this->mana_current_val = new_val;
    this->mana_max_val = new_val;
    this->mana_regen_rate = new_val;
    this->mana_regen_interval = new_val; 

    this->armor_current_val = new_val;
    this->armor_max_val = new_val;
    this->armor_regen_rate = new_val;
    this->armor_regen_interval = new_val; 

    this->damage_current_val = new_val;
    this->damage_max_val = new_val;
    this->damage_regen_rate = new_val;
    this->damage_regen_interval = new_val; 

};

void ItemEffect::set_health_vals_to(int new_val)
{
    this->health_current_val = new_val;
    this->health_max_val = new_val;
    this->health_regen_rate = new_val;
    this->health_regen_interval = new_val; 
};

void ItemEffect::set_mana_vals_to(int new_val)
{
    this->mana_current_val = new_val;
    this->mana_max_val = new_val;
    this->mana_regen_rate = new_val;
    this->mana_regen_interval = new_val; 
};

void ItemEffect::set_armor_vals_to(int new_val)
{
    this->armor_current_val = new_val;
    this->armor_max_val = new_val;
    this->armor_regen_rate = new_val;
    this->armor_regen_interval = new_val; 
};

void ItemEffect::set_damage_vals_to(int new_val)
{
    this->damage_current_val = new_val;
    this->damage_max_val = new_val;
    this->damage_regen_rate = new_val;
    this->damage_regen_interval = new_val; 
};

bool ItemEffect::already_applied(Actor* actor)
{

    //if actor in list of people this item's already applied to, no reason to do
    //it again
    auto it = std::find(this->applied_to->begin(), this->applied_to->end(), actor);

    return  it != this->applied_to->end();

    // The thing is that I need to sort out how to apply the item's effects to
    // the actor once, because if it gets reapplied they might be invincible or
    // gaining way too much health for ever.
    //TODO TODO TODO

};

void ItemEffect::mark_applied_to(Actor* actor)
{
    auto it = std::find(this->applied_to->begin(), this->applied_to->end(), actor);

    if ( it != this->applied_to->end() )
    {
        //do nothing because it already exists in the vector
    }
    else
    {
        this->applied_to->push_back(actor);
    };
};

void ItemEffect::ApplyAllEffects(Actor* actor)
{
    this->ApplyHealthEffects(actor);
    this->ApplyManaEffects(actor);
    this->ApplyArmorEffects(actor);
    this->ApplyDamageEffects(actor);
};

void ItemEffect::ApplyHealthEffects(Actor* actor)
{
    if (! this->already_applied(actor))
    {
        actor->attrs->health->AddToCurrentVal(this->health_current_val);
        actor->attrs->health->AddToMaxVal(this->health_max_val);
        actor->attrs->health->AddToRegenRate(this->health_regen_rate);
        actor->attrs->health->AddToRegenInterval(this->health_regen_interval);
        this->mark_applied_to(actor);
    }
};

void ItemEffect::ApplyManaEffects(Actor* actor)
{
    if (! this->already_applied(actor))
    {
        actor->attrs->mana->AddToCurrentVal(this->mana_current_val);
        actor->attrs->mana->AddToMaxVal(this->mana_max_val);
        actor->attrs->mana->AddToRegenRate(this->mana_regen_rate);
        actor->attrs->mana->AddToRegenInterval(this->mana_regen_interval);
        this->mark_applied_to(actor);
    }
};

void ItemEffect::ApplyArmorEffects(Actor* actor)
{
    if (! this->already_applied(actor))
    {
        actor->attrs->armor->AddToCurrentVal(this->armor_current_val);
        actor->attrs->armor->AddToMaxVal(this->armor_max_val);
        actor->attrs->armor->AddToRegenRate(this->armor_regen_rate);
        actor->attrs->armor->AddToRegenInterval(this->armor_regen_interval);
        this->mark_applied_to(actor);
    }
};

void ItemEffect::ApplyDamageEffects(Actor* actor)
{
    if (! this->already_applied(actor))
    {
        actor->attrs->damage->AddToCurrentVal(this->damage_current_val);
        actor->attrs->damage->AddToMaxVal(this->damage_max_val);
        actor->attrs->damage->AddToRegenRate(this->damage_regen_rate);
        actor->attrs->damage->AddToRegenInterval(this->damage_regen_interval);
        this->mark_applied_to(actor);
    }

};

void ItemEffect::RemoveAllEffects(Actor* actor)
{
    this->RemoveHealthEffects(actor);
    this->RemoveManaEffects(actor);
    this->RemoveArmorEffects(actor);
    this->RemoveDamageEffects(actor);
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


std::string ItemEffect::full_str()
{

    std::string result = "";
    std::vector<std::string> string_vec;

    string_vec.push_back("HCV: "+std::to_string((long double)this->health_current_val));
    string_vec.push_back("HMV: "+std::to_string((long double)this->health_max_val));
    string_vec.push_back("HRR: "+std::to_string((long double)this->health_regen_rate));
    string_vec.push_back("HRI: "+std::to_string((long double)this->health_regen_interval));

    string_vec.push_back("MCV: "+std::to_string((long double)this->mana_current_val));
    string_vec.push_back("MMV: "+std::to_string((long double)this->mana_max_val));
    string_vec.push_back("MRR: "+std::to_string((long double)this->mana_regen_rate));
    string_vec.push_back("MRI: "+std::to_string((long double)this->mana_regen_interval));

    string_vec.push_back("ACV: "+std::to_string((long double)this->armor_current_val));
    string_vec.push_back("AMV: "+std::to_string((long double)this->armor_max_val));
    string_vec.push_back("ARR: "+std::to_string((long double)this->armor_regen_rate));
    string_vec.push_back("ARI: "+std::to_string((long double)this->armor_regen_interval));

    string_vec.push_back("DCV: "+std::to_string((long double)this->damage_current_val));
    string_vec.push_back("DMV: "+std::to_string((long double)this->damage_max_val));
    string_vec.push_back("DRR: "+std::to_string((long double)this->damage_regen_rate));
    string_vec.push_back("DRI: "+std::to_string((long double)this->damage_regen_interval));

    return StringJoin(string_vec, '\n', false);

};

std::string ItemEffect::small_convert(std::string prefix, long double val)
{
    if (val != 0)
    {
        return prefix + std::to_string(val);
    }
    else
    {
        return "";
    };

};

std::string ItemEffect::oneline_str()
{

    std::string result = "";
    std::vector<std::string> string_vec;

    string_vec.push_back(this->small_convert("HCV: ", (long double)this->health_current_val));
    string_vec.push_back(this->small_convert("HMV: ", (long double)this->health_max_val));
    string_vec.push_back(this->small_convert("HRR: ", (long double)this->health_regen_rate));
    string_vec.push_back(this->small_convert("HRI: ", (long double)this->health_regen_interval));

    string_vec.push_back(this->small_convert("MCV: ", (long double)this->mana_current_val));
    string_vec.push_back(this->small_convert("MMV: ", (long double)this->mana_max_val));
    string_vec.push_back(this->small_convert("MRR: ", (long double)this->mana_regen_rate));
    string_vec.push_back(this->small_convert("MRI: ", (long double)this->mana_regen_interval));

    string_vec.push_back(this->small_convert("ACV: ", (long double)this->armor_current_val));
    string_vec.push_back(this->small_convert("AMV: ", (long double)this->armor_max_val));
    string_vec.push_back(this->small_convert("ARR: ", (long double)this->armor_regen_rate));
    string_vec.push_back(this->small_convert("ARI: ", (long double)this->armor_regen_interval));

    string_vec.push_back(this->small_convert("DCV: ", (long double)this->damage_current_val));
    string_vec.push_back(this->small_convert("DMV: ", (long double)this->damage_max_val));
    string_vec.push_back(this->small_convert("DRR: ", (long double)this->damage_regen_rate));
    string_vec.push_back(this->small_convert("DRI: ", (long double)this->damage_regen_interval));

    return StringJoin(string_vec, '|', true);

};

