#ifndef SKELETON_H
#define SKELETON_H

#include <actors\Person.h>

class Skeleton : public Person
{
    public:
        Skeleton(std::string name, int age, int x, int y, char repr, std::string combat_name);
        static const int pack_size = 10;
        static const int preferred_pack_size = 5;
        void championize();
};

#endif
