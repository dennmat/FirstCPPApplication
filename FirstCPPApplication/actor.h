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

        TCODPath* l_path;

        Actor::Actor()
        {
            l_path = NULL;
            representation = new Representation;
            name = "Unset Actor name";
        };
        std::string Actor::GetName();
        virtual void Actor::update() = 0;

};

#endif
