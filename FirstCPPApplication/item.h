#ifndef ITEM_H
#define ITEM_H

#include "equipment.h"

class Representation;
class Slot;

// enum slots_t : unsigned int;

class Item
{
    public:
        Representation* repr;
        Item();

        slots_t slot_type;

};

#endif
