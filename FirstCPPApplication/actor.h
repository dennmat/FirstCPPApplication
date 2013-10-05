#ifndef ACTOR_H
#define ACTOR_H

#include "Representation.h"
#include <string>
#include <vector>
#include "tile.h"

class Representation;

class Tile;
class Thinker;

class Actor : public Object 
{

    public:
        bool is_fighter;
        std::string name;
        Representation* representation;
        Thinker* thinker;

        TCODPath* l_path;

        int x,y;
        int dest_x, dest_y;
        Tile * my_tile;

        bool is_moving_left;
        bool is_moving_right;
        bool is_moving_up;
        bool is_moving_down;

        std::vector<Actor*> * actors_in_sight;

        Actor();
        std::string GetName();
        const char* GetNameC();
        virtual void update() = 0;
        void ActorInSight(int x, int y, Actor * actor);
        void putPerson( Tile * next_tile, int new_x, int new_y);

};

#endif
