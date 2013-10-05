#include "stdafx.h"
#include "thinker.h"

#include <iostream>
#include <algorithm>

#include "map.h"
#include "libtcod.hpp"
#include "Person.h"
#include "game.h"

using namespace std;

Thinker::Thinker()
{

};

void Thinker::update(Game* game)
{

    // vector<Actor*>::iterator aisItr;
    // aisItr = std::find(game->player->actors_in_sight->begin(), game->player->actors_in_sight->end(),  this->master);
    if (game->player->IsActorInSight(this->master))
    {
        printf("thinking\n");
        //PATH FINDING
        Map* map = master->my_tile->map;
        if (master->l_path == NULL)
        {
            master->l_path = new TCODPath(map->l_map);

            TCODRandom * rnd = TCODRandom::getInstance();
            master->dest_x = rnd->getInt(1, map->width-1);
            master->dest_y = rnd->getInt(1, map->height-1);
            master->l_path->compute(master->x, master->y, master->dest_x, master->dest_y);
        }
        else
        {
            cout << "Path size: " << master->l_path->size() << endl;
            master->l_path->walk(&master->x, &master->y, false);
            Tile * next_tile = map->getTileAt(master->x,master->y);
            master->putPerson(next_tile, master->x, master->y); 
        };
    }

};
