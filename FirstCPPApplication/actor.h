#ifndef ACTOR_H
#define ACTOR_H

#include "Representation.h"
#include <string>
#include <vector>
#include "tile.h"

class Representation;

class Tile;
class Thinker;
class Game;
class Inventory;
class Equipment;

class Actor : public Object 
{

    public:
        std::string name;
        Representation* representation;
        Thinker* thinker;
        Inventory* inventory;
        Equipment* equipment;

        TCODPath* l_path;

        int x,y;
        int dest_x, dest_y;
        Tile * my_tile;

        bool is_moving_left;
        bool is_moving_right;
        bool is_moving_up;
        bool is_moving_down;
        void resetIsMoving();

        std::vector<Actor*> * actors_in_sight;

        bool is_fighter;
        bool has_attacked;

        Actor();
        std::string GetName();
        const char* GetNameC();

        virtual void update(Game* game) = 0;

        bool IsActorInSight(Actor * actor);
        void ActorInSight(int x, int y, Actor * actor);

        void putPerson( Tile * next_tile, int new_x, int new_y);

        void pickUpItem(Item* item);

};

#endif
