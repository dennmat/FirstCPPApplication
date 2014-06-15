#pragma once
#ifndef SKILLS_H
#define SKILLS_H

class Actor;

class Skill
{
    std::string name;
    std::string description;

    Actor* master;
    
    Skill();

    bool can_apply();
    void apply();
    void update();

};

#endif
