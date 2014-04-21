#pragma once
#ifndef CLASS_H
#define CLASS_H

class Actor;

class IClass
{
    public:
        Actor* master;
        std::string name;
        virtual ~IClass() {};
        virtual void LevelUpStats(int levels) = 0;
        virtual void LevelUpSkills(int levels) = 0;

        void LevelUpHealth(int change);
        void LevelUpMana(int change);
        void LevelUpArmor(int change);
        void LevelUpDamage(int change);
};

class FighterClass : public IClass
{
    public:
        FighterClass();
        virtual void LevelUpStats(int levels);
        virtual void LevelUpSkills(int levels);
};

class MageClass : public IClass
{
    public:
        MageClass();
        virtual void LevelUpStats(int levels);
        virtual void LevelUpSkills(int levels);
};

class BrawlerClass : public IClass
{
    public:
        BrawlerClass();
        virtual void LevelUpStats(int levels);
        virtual void LevelUpSkills(int levels);
};


#endif
