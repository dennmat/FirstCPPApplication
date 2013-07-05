#pragma once

#include "Person.h"
#include "Pet.h"
#include "world.h"
#include "stdafx.h"


class Game
{
    public:
        Person *player;
        Person *enemies[];
};
