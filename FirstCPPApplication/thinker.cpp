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
        Map* map = game->player->my_tile->map;
        delete master->l_path;
        master->l_path = NULL;
        if (master->l_path == NULL)
        {
            cout << "Building Path" << endl;
            master->l_path = new TCODPath(map->l_map);

            //set the master's destination to above the player
            vector<Tile*>* adjacent_tiles = game->player->my_tile->getVacantAdjacentTiles();
            std::random_shuffle(adjacent_tiles->begin(), adjacent_tiles->end());
            master->dest_x = adjacent_tiles->back()->tile_x;
            master->dest_y = adjacent_tiles->back()->tile_y;

            int x, y, dest_x, dest_y;
            x = master->x;
            y = master->y;
            dest_x = master->dest_x;
            dest_y = master->dest_y;

            //compute a path for the master to walk
            bool computed = master->l_path->compute(x, y, dest_x, dest_y);
            int path_size = master->l_path->size();

            cout << "Path size AFTER BUILDING IT: " << path_size << endl << "I'mna walk it" << endl;
        };

        int path_size = master->l_path->size();
        cout << "Path size: " << path_size << endl << "I'mna walk it" << endl;

        master->l_path->walk(&master->x, &master->y, true);
        Tile * next_tile = map->getTileAt(master->x,master->y);
        master->putPerson(next_tile, master->x, master->y); 

        //if (path_size == 0)
        //{
        //}
    }

};
