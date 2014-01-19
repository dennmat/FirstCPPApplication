#include "stdafx.h"
#include "thinker.h"

#include <iostream>
#include <algorithm>

#include "libtcod.hpp"
#include "map.h"
#include <actors\Person.h>
#include "game.h"
#include "Representation.h"
#include "tile.h"	
#include "combat.h"

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
        // printf("thinking\n");
        //PATH FINDING
        Map* map = game->player->my_tile->map;

        //in order for the AI to keep looking for the player, the path needs to
        //be rebuilt every update, otherwise he just walks to the same place
        //until he gets there
        if (master->l_path != NULL)
        {
            //get the destination tile coords
            int dest_tile_x, dest_tile_y;
            Tile* player_tile = game->player->my_tile;
            master->l_path->getDestination(&dest_tile_x, &dest_tile_y);

            //if the target tile is adjacent to the player keep moving towards
            //it, otherwise change spots
            vector<Tile*>* adj_tiles = game->world->getTileAt(dest_tile_x, dest_tile_y)->getVacantAdjacentTiles();
            vector<Tile*>::iterator adjItr = std::find(adj_tiles->begin(), adj_tiles->end(), player_tile);
            if (adjItr == adj_tiles->end())
            {

                //if the path destination isn't adj to the player make a new path
                // cout << "no adjacent tiles found next to player where I'm pathing to, so I'm making a new path" << endl;
                delete master->l_path;
                master->l_path = NULL;
            }
            else
            {
                //continue on that path
            };


        };

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

            // cout << "Path size AFTER BUILDING IT: " << path_size << endl << "I'mna walk it" << endl;
        };

        int path_size = master->l_path->size();
        // cout << "Path size: " << path_size << endl << "I'mna walk it" << endl;

        master->l_path->walk(&master->x, &master->y, true);
        Tile * next_tile = map->getTileAt(master->x,master->y);
        master->putPerson(next_tile, master->x, master->y); 

        if (path_size == 0)
        {
            // cout << "IMNA ATTACK THE PLAYER" << endl;
            //attack the player if he's in range (aka adjacent tile)
            Combat* assailant = game->player->combat;
            vector<Tile*>* adjacent_tiles = ((Person*)master)->my_tile->getAdjacentTiles();
            if (std::find(adjacent_tiles->begin(), adjacent_tiles->end(),
                        assailant->master->my_tile) != adjacent_tiles->end())
            {
                // std::cout << "adjacent" << std::endl;
                ((Person*)master)->combat->Attack(assailant, 1);

            };

            //calm the combat down
            ((Person*)master)->combat->was_attacked = false;
        }
    }

};
