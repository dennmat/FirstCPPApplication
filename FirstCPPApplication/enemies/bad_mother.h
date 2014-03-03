#ifndef BAD_MOTHER_H
#define BAD_MOTHER_H

#include <actors\Person.h>

class BadMother : public Person
{
    public:
        BadMother(std::string name, int age, int x, int y, char repr, std::string combat_name);
        static const int BadMother::pack_size = 1;
        void championize();
	void Die();

};

#endif
