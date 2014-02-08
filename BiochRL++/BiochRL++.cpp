// BiochRL++.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.h"
#include "map.h"


	//Game the_game;
int _tmain(int argc, _TCHAR* argv[])
{
auto map = Game::buildworld();
Game::current_map = map;
//Map* Game::current_map = world; //I'm not so sure about this, but it solved the mem issue

Game::initialize_player(); //created the Person player
Game::initialize_enemies(); // create the enemies
Game::initialize_items(); // create the items
	Game::mainloop();
	return 0;
}

