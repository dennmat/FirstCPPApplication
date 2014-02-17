#ifndef OGRE_H
#define OGRE_H

#include <actors\Person.h>

class Ogre : public Person
{
    public:
        Ogre(std::string name, int age, int x, int y, char repr, std::string combat_name);
};

#endif
