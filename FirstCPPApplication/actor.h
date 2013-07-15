#ifndef ACTOR_H
#define ACTOR_H

#include "object.h"
#include "Representation.h"

class Representation;

class Actor : public Object 
{

    public:
        bool is_fighter;
        Representation* representation;

        Actor::Actor()
        {
            representation = new Representation;
        };

};

#endif
