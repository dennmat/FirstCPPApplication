#pragma once
#ifndef ATTRIBUTE_CONTAINER_H
#define ATTRIBUTE_CONTAINER_H


// class Representation;
// class Tile;
// class Thinker;
// class Game;
// class Inventory;
// class Equipment;
// class Item;
class HealthAttribute;
class ManaAttribute;
class ArmorAttribute;
class DamageAttribute;
class Actor;

class AttributeContainer
{
    public:
        Actor* owner;

        HealthAttribute* health;
        ManaAttribute* mana;
        ArmorAttribute* armor;
        DamageAttribute* damage;

        AttributeContainer();

        void RegenerateAll();
        void RegenerateHealth();
        void RegenerateMana();
        void RegenerateArmor();
        void RegenerateDamage();
        void Update();

};

#endif
