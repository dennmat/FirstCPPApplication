#ifndef JACKAL_H
#define JACKAL_H

#include <actors\Person.h>

class Jackal : public Person
{
    public:
        Jackal(std::string name, int age, int x, int y, char repr, std::string combat_name);
        static const int pack_size = 10;
        static const int preferred_pack_size = 10;
        void championize();

};

#endif
