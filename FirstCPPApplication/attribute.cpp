#include "stdafx.h"
#include "attribute.h"

Attribute::Attribute()
{
    this->tick = 0;
    this->is_ready = false;
    this->current_val = 999;
    this->max_val = 999;
    this->regen_rate = 1;
    this->regen_interval = 1;

};


void Attribute::Update()
{
    this->Tick();

    if (this->CheckIsReady())
    {
        this->AddToCurrentVal(this->regen_rate);
    };

};

void Attribute::Tick()
{

    //if there's no regen rate, freeze and reset regen
    if (this->regen_rate == 0)
    {
        this->is_ready = false;
        this->tick = 0;
        return;
    };

    if (this->tick < this->regen_interval)
    {
        this->tick += 1;
        this->is_ready = false;
    }
    else
    {
        this->is_ready = true;
    };

};

void Attribute::AddToCurrentVal(int difference, bool force_change)
{
    if (difference != 0)
    {

        this->current_val += difference;
        if (this->current_val > this->max_val && !force_change) 
        {
            this->current_val = this->max_val;
        };
    };

};

bool Attribute::CheckIsReady()
{
    return this->is_ready;
};

Health::Health() : Attribute()
{
    this->max_val = 100;
    this->current_val = this->max_val;

};

Mana::Mana() : Attribute()
{
    this->max_val = 25;
    this->current_val = this->max_val;

    this->regen_rate = 1;
    this->regen_interval = 3;

};

Armor::Armor() : Attribute()
{
    this->max_val = 1;
    this->current_val = this->max_val;

    this->regen_rate = 0;
};

Damage::Damage() : Attribute()
{
    this->max_val = 5;
    this->current_val = this->max_val;

    this->regen_rate = 0;

};
