#ifndef ACTOR_H
#define ACTOR_H

#include "object.h"
#include "Representation.h"
#include <string>
#include <vector>

class Representation;

class Actor;

class Actor : public Object 
{

    public:
        bool is_fighter;
        std::string name;
        Representation* representation;

        TCODPath* l_path;

        bool is_moving_left;
        bool is_moving_right;
        bool is_moving_up;
        bool is_moving_down;

        std::vector<Actor*> * actors_in_sight;

        Actor::Actor()
        {
            l_path = NULL;
            representation = new Representation;
            name = "Unset Actor name";

            actors_in_sight = new std::vector<Actor*>;

            is_moving_left = false;
            is_moving_right = false;
            is_moving_up = false;
            is_moving_down = false;
        };
        std::string Actor::GetName();
        const char* Actor::GetNameC();
        virtual void Actor::update() = 0;
        void Actor::ActorInSight(int x, int y, Actor * actor);

};

#endif
