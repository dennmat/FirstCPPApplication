#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>

#include "object.h"

class Representation;

class Tile;
class Thinker;
class Game;
class Inventory;
class Equipment;
class Item;
class AttributeContainer;

class TCODPath;



class Actor : public Object 
{

    public:
        std::string name;
        Representation* representation;
        Thinker* thinker;
        Inventory* inventory;
        Equipment* equipment;

        AttributeContainer* attrs;
        bool has_attributes();
        bool is_active;

        TCODPath* l_path;

        int x,y;
        int dest_x, dest_y;
        Tile * my_tile;

        int xp;
        int xp_this_level;
        int xp_required;

        int level;
        int xp_value;
        

        bool is_moving_left;
        bool is_moving_right;
        bool is_moving_up;
        bool is_moving_down;
        void resetIsMoving();
        int* get_direction_heading();

        std::vector<Actor*> * actors_in_sight;

        bool is_fighter;
        bool has_attacked;

        Actor();
        std::string GetName();
        const char* GetNameC();

        virtual void update(Game* game) = 0;

        Item* Actor::CreateCorpse();
        void Die();

        bool IsActorInSight(Actor * actor);
        void ActorInSight(int x, int y, Actor * actor);

        void putPerson( Tile * next_tile, int new_x, int new_y);

        void pickUpItem(Item* item);

};

#endif
