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
        this->Regenerate();
        // this->AddToCurrentVal(this->regen_rate);
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

    if (this->tick <= this->regen_interval)
    {
        this->tick += 1;
        this->is_ready = false;
    }
    else
    {
        this->tick = 0;
        this->is_ready = true;
    };

};

void Attribute::AddToCurrentVal(int difference)
{
    if (difference != 0)
    {
        this->current_val += difference;
    };

};

void Attribute::AddToMaxVal(int difference)
{
    if (difference != 0)
    {
        this->max_val += difference;
    };

};

void Attribute::AddToRegenRate(int difference)
{
    if (difference != 0)
    {
        this->regen_rate += difference;
    };

};

void Attribute::AddToRegenInterval(int difference)
{
    if (difference != 0)
    {
        this->regen_interval += difference;
    };

};

void Attribute::RemoveFromCurrentVal(int difference)
{
    if (difference != 0)
    {

        this->current_val -= difference;
    };

};

void Attribute::RemoveFromMaxVal(int difference)
{
    if (difference != 0)
    {
        this->max_val -= difference;
    };

};

void Attribute::RemoveFromRegenRate(int difference)
{
    if (difference != 0)
    {
        this->regen_rate -= difference;
    };

};

void Attribute::RemoveFromRegenInterval(int difference)
{
    if (difference != 0)
    {
        this->regen_interval -= difference;
    };

};

bool Attribute::CanRegenerate()
{
    //TODO add check to see if has enough intervals has been passed
    return true;
};

void Attribute::Regenerate()
{
    if (this->CanRegenerate())
    {
        this->current_val += this->regen_rate;
        if (this->current_val > this->max_val)
        {
            this->current_val = this->max_val;
        }
    };

};

bool Attribute::CheckIsReady()
{
    return this->is_ready;
};

HealthAttribute::HealthAttribute() : Attribute()
{
    this->max_val = 100;
    this->current_val = this->max_val;
    this->regen_interval = 3;

};

ManaAttribute::ManaAttribute() : Attribute()
{
    this->max_val = 25;
    this->current_val = this->max_val;

    this->regen_rate = 1;
    this->regen_interval = 3;

};

ArmorAttribute::ArmorAttribute() : Attribute()
{
    this->max_val = 1;
    this->current_val = this->max_val;

    this->regen_rate = 0;
};

DamageAttribute::DamageAttribute() : Attribute()
{
    this->max_val = 5;
    this->current_val = this->max_val;

    this->regen_rate = 0;

};
