// BiochRL++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.h"
#include "map.h"


int _tmain(int argc, _TCHAR* argv[])
{
    auto map = Game::buildworld();
    Game::current_map = map;

    Game::initialize_player(); //created the Person player
    //Game::initialize_enemies(); // create the enemies
    //Game::initialize_items(); // create the items
    Game::mainloop();
    return 0;
}

