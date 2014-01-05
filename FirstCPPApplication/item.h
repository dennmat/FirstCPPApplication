#ifndef ITEM_H
#define ITEM_H

#include <string>

// #include "equipment.h"
#include <enums\slots_t.h>

class Representation;
class Slot;
class ItemEffect;
class Actor;

// enum slots_t : unsigned int;

class Item
{
    public:
        std::string description;
        std::string name;

        ItemEffect* item_effect;
        Representation* repr;

        Item();
        void use(Actor* target);

        slots_t slot_type;

};

#endif
