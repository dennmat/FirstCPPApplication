#include "stdafx.h"
#include <vector>
#include <algorithm>

#include "inventory.h"
#include "item.h"

Inventory::Inventory()
{
    this->items = new std::vector<Item*>;
};

void Inventory::add_item(Item* item)
{

    //check for item in list, if its not, add it in
    auto it = std::find(this->items->begin(), this->items->end(), item);
    if (it == this->items->end())
    {
        this->items->push_back(item);
    };

};

void Inventory::remove_item(Item* item)
{
    //check for item in list, if its not, remove it 
    auto it = std::find(this->items->begin(), this->items->end(), item);
    if (it != this->items->end())
    {
        this->items->erase(it);
    };

};
int Inventory::get_count()
{
    return this->items->size();
};
