#ifndef TROLL_H
#define TROLL_H

#include <actors\Person.h>

class Troll : public Person
{
    public:
        Troll(std::string name, int age, int x, int y, char repr, std::string combat_name);
        static const int Troll::pack_size = 10;
        void championize();
        void Die() ;

};

#endif
