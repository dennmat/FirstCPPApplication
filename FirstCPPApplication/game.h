#pragma once

#include "stdafx.h"
#include "Person.h"
#include "Pet.h"
#include "world.h"


class Game
{
    public:
        Person *player;     //the PC

        int enemies_size ;   //don't really know how else to get the size of the
                            // enemies list.  sizeof(type_inst_array)/sizeof(type) maybe.
        Person *enemies[];  //later, this will be an array of all the enemies 

        Game::Game();
};
