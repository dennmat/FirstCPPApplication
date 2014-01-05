#include "stdafx.h"


#include "attribute_container.h"
#include <actors/actor.h>
#include "attribute.h"


AttributeContainer::AttributeContainer()
{
    this->owner = NULL;

    this->health = new HealthAttribute;
    this->mana = new ManaAttribute;
    this->armor = new ArmorAttribute;
    this->damage = new DamageAttribute;
};

void AttributeContainer::RegenerateAll()
{
    this->RegenerateHealth();
    this->RegenerateMana();
    this->RegenerateArmor();
    this->RegenerateDamage();
};

void AttributeContainer::RegenerateHealth()
{
    this->health->Regenerate();

};

void AttributeContainer::RegenerateMana()
{
    this->mana->Regenerate();

};

void AttributeContainer::RegenerateArmor()
{
    this->armor->Regenerate();

};

void AttributeContainer::RegenerateDamage()
{
    this->damage->Regenerate();

};
