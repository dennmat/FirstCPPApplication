#ifndef ACTOR_H
#define ACTOR_H

#include "object.h"
#include "Representation.h"
#include <string>

class Representation;

class Actor : public Object 
{

    public:
        bool is_fighter;
        std::string name;
        Representation* representation;

        Actor::Actor()
        {
            representation = new Representation;
            name = "Unset Actor name";
        };
        virtual void Actor::update() = 0;

};

#endif
