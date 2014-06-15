#include "stdafx.h"
#include "skills.h"


Skill::Skill()
{
    this->name = "Unamed skill";
    this->description = "Undescribed skill";
    this->master = NULL;
};

bool Skill::can_apply()
{
    return true;
};

void Skill::apply()
{

};

void Skill::update()
{
    if (this->can_apply())
    {
        this->apply();
    };
};
