#ifndef ITEM_H
#define ITEM_H

#include <string>

#include "equipment.h"


class Representation;
class Slot;
class ItemEffect;

// enum slots_t : unsigned int;

class Item
{
    public:
        std::string description;
        std::string name;


        ItemEffect* item_effect;
        Representation* repr;
        Item();

        slots_t slot_type;

};

#endif
