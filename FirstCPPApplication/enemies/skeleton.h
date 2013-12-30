#ifndef SKELETON_H
#define SKELETON_H

#include <actors\Person.h>

class Skeleton : public Person
{
    public:
        Skeleton(std::string name, int age, int x, int y, char repr, std::string combat_name);
};

#endif
