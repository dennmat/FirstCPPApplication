#ifndef ATTRBUTE_H
#define ATTRBUTE_H

class Actor;
class Person;
class Pet;
class Person;
class Item;
class Inventory;
class Tile;

class Attribute 
{
    private:
        int tick;
        bool is_ready;

    public:
        int current_val;
        int max_val;

        int regen_rate; //per turn
        int regen_interval; //per x turns? 

        Attribute();


        virtual void Update();
        virtual void Tick();
        virtual bool CheckIsReady();

        virtual void AddToCurrentVal(int difference);

};

class Health : Attribute
{

    Health();

};

class Mana : Attribute
{
    Mana();
};

class Armor : Attribute
{
    Armor();
};

class Damage : Attribute
{
    Damage();

};

#endif
