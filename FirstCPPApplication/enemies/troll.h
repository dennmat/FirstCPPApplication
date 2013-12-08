#ifndef TROLL_H
#define TROLL_H

#include "Person.h"

class Troll : public Person
{
    public:
        Troll(std::string name, int age, int x, int y, char repr, std::string combat_name);

};

#endif
