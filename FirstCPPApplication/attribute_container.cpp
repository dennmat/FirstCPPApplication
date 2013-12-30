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

