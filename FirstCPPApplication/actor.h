#ifndef ACTOR_H
#define ACTOR_H

#include "object.h"

class Actor : public Object 
{

    public:
        bool is_fighter;
        char representation;
};

#endif
