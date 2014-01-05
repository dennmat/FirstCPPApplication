#ifndef INVENTORY_H
#define INVENTORY_H

class Item;
class Tile;
class Actor;

class Inventory
{
    public:
    
        Actor* master;

        std::vector<Item*>* items;

        void add_item(Item*);
        void remove_item(Item*);
        int get_count();
        Inventory();

        void drop_item(Item* item);

};

#endif
