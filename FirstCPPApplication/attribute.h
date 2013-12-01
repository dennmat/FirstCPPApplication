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
        virtual void AddToMaxVal(int difference);
        virtual void AddToRegenRate(int difference);
        virtual void AddToRegenInterval(int difference);

        virtual void RemoveFromCurrentVal(int difference);
        virtual void RemoveFromMaxVal(int difference);
        virtual void RemoveFromRegenRate(int difference);
        virtual void RemoveFromRegenInterval(int difference);

};

class HealthAttribute : public Attribute
{
    public:
        HealthAttribute();

};

class ManaAttribute : public Attribute
{
    public:
        ManaAttribute();
};

class ArmorAttribute : public Attribute
{
    public:
        ArmorAttribute();
};

class DamageAttribute : public Attribute
{
    public:
        DamageAttribute();

};

#endif
