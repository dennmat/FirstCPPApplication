#ifndef INVENTORY_H
#define INVENTORY_H

class Item;
class Tile;

class Inventory
{
    public:
        std::vector<Item*>* items;

        void add_item(Item*);
        void remove_item(Item*);
        int get_count();
        Inventory();

        void Inventory::drop_item(Item* item, Tile* tile);

};

#endif
