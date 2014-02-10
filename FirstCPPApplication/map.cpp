#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "libtcod.hpp"
#include <vector>
#include <algorithm>

#include "utils.h"
#include "map.h"
#include "Room.h"
#include <actors\Person.h>
#include "game.h"
#include "item.h"
#include "inventory.h"
#include "Representation.h"
#include "tile.h"
#include "enemies\troll.h"
#include "debug_options.h"
#include "messages.h"
#include "ui.h"

using namespace std;

Map::Map()
{
    this->enemies = vector<Actor*>();
    this->roomVector = new vector<Room*>;

    this->width = NULL;
    this->height = NULL;

}

Map::~Map()
{

}

Tile * Map::getTileAt(int x, int y, bool is_original_pos, int ox, int oy)
{
    vector<Tile> * x_tiles;

    if (x < 0  )
    {
        x = 0;
    };

    if ( y < 0  )
    {
        y = 0;
    };

    if (!is_original_pos && (x < 0 || y < 0 ) ) // this shouldnt ever happen now
    { 
        cout << "can't find a tile here.";
        cout << "I'd throw and error but I don't know how yet" << endl;
    }

    // these two ifs cut way the hell down on recursion by eliminating the
    // need entirely I think
    if (x >= this->width)
    {
        x = this->width-1;
    };

    if (y >= this->height)
    {
        y = this->height-1;
    };


    //try to get the y vector
    try { x_tiles = &tileVector->at(y); }
    catch ( std::out_of_range& ex )
    {
        ex;
        std::cout << "getTileAt recursed on y" << std::endl;
        return getTileAt(x, y-1, false, x, y);
    };

    //try to get the tile from the x vector in the y vector
    // and sense an message if the position isn't the same as the original position
    try 
    {
        auto found_tile = &x_tiles->at(x);
        //auto found_tile2=  &(*tileVector)[y][x]; 
        return found_tile;
    }
    catch ( std::out_of_range& ex )
    {
        ex;
        std::cout << "getTileAt recursed on x" << std::endl;
        return getTileAt(x-1, y, false, x, y);
    };
};

class BspListener : public ITCODBspCallback 
{
    private:
        Map& map;
        int roomNum;
        int lastx, lasty;

    public:
        static std::stringstream output;
        BspListener(Map &map) : map(map), roomNum(0) {};
        bool visitNode(TCODBsp *node, void *userData)
        {
            if (node->isLeaf())
            {
                // std::cout << "nodes a leaf" << std::endl;

                int room_x,room_y,room_w,room_h;
                int room_min_size = 6;
                // dig a room
                TCODRandom *rng = TCODRandom::getInstance();

                room_w = rng->getInt(room_min_size, node->w-2);
                room_h = rng->getInt(room_min_size, node->h-2);
                room_x = rng->getInt(node->x+1, node->x+node->w-(room_w-1));
                room_y = rng->getInt(node->y+1, node->y+node->h-(room_h-1));

                int perimeter = room_w*2 + room_h*2 - 4;
                int door_index = rng->getInt(0, perimeter);
                map.build_rect_room(room_x, room_y, room_w, room_h, door_index);

                lastx=room_x+room_w/2;
                lasty=room_y+room_h/2;
                roomNum++;

                Tile* door_tile = map.getTileAt(room_x+1, room_y+1);
                door_tile->updateTileType(5);
                map.l_map->setProperties(door_tile->tile_x, door_tile->tile_y, true, true);
            }
            else
            {
                BspListener::output << "nodes NOT A leaf " << std::endl;
                // std::cout << "nodes NOT A leaf " << std::endl;

                Tile* tile;
                TCODRandom *rng = TCODRandom::getInstance();
                int x, y;
                for (int i = 0; i < 16; i++)
                {
                    x = rng->getInt(node->x+1, node->x+node->w -3);
                    y = rng->getInt(node->y+1, node->y+node->h -3);
                    tile= map.getTileAt(x, y);
                    if ( tile->tile->type_id == TileTypes::FloorTileTypeType)
                    {
                        tile->tile->representation->repr = ',';
                        tile->tile->representation->setFGColor(TCODColor::darkerGrey, true, true, true);
                        tile->tile->description = "A small stone lays here";
                    }
                }
            }
            return true;
        };
};
std::stringstream BspListener::output = std::stringstream();

int Map::build_from_random(int seed)
{
    width = Game::map_width;
    height = Game::map_height;
    l_map = new TCODMap(width, height);
    //the default tile description
    description = "poppycock";

    tileVector = new vector<vector<Tile>>;
    tileVector->resize(height);
    for(int ix = 0; ix < height; ++ix)
    {
        (*tileVector)[ix].resize(width);
    }

    int i = 0;
    int x = 0;
    int y = 0;

    int room_max_x = 12;
    int room_max_y = 6;

    while ( i < width*height )
    {
        Tile* this_tile = getTileAt(x, y);
        this_tile->map = this;
        this_tile->updateTileType(TileTypes::FloorTileTypeType);
        if(this_tile->type_id == TileTypes::FloorTileTypeType)
        {
            //light passes though, walkable
            l_map -> setProperties(x, y, true, true);
        }

        else 
        {
            //light does NOT pass through nor is walkable
            l_map -> setProperties(x, y, false, false);
        }

        this_tile->tile->description = "Descriptionless tile";

        this_tile->tile_x = x;
        this_tile->tile_y = y;

        if ( x >= (width -1)  ) // width is 1, only tile would be (0, 0) so you need to substract 1
        {
            y++;
            x = 0;
        }
        else { 
            x++;
        };

        i++;
    }

    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, room_max_x, room_max_y, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, this);

    std::cout << "" << BspListener::output.str() << std::endl;

    return 1;

};


void Map::build_rect_room(int room_x, int room_y,
        int room_width, int room_height,
        int door_index)
{
    //create and add the room to the list of vectors
    Room * room = new Room(room_x, room_y, room_width, room_height, door_index);
    this->roomVector->push_back(room);

    for(int new_y=0; new_y<room_height; new_y++)
    {
        for(int new_x=0; new_x<room_width;new_x++)
        {
            // std::cout << new_x << " x y " << new_y << std::endl;
            int adj_x = room_x + new_x;
            int adj_y = room_y + new_y;

            //done to save time later on for getTileAt recursion
            if (adj_y >= this->height)
            {
                // break;
                adj_y = this->height-2;
            }

            if (adj_y >= this->width)
            {
                // break;
                adj_y = this->width-2;
            }

            //check for outer perimeter
            if (room->isPerimeter(new_x, new_y))
            {
                getTileAt(adj_x, adj_y)->updateTileType(TileTypes::WallTileTypeType); //for wall
                l_map -> setProperties(adj_x, adj_y, false, false);

                //place door if valid position
                if (room->checkDoorCount())
                {
                    getTileAt(adj_x, adj_y)->updateTileType(TileTypes::DoorTileTypeType); //for door
                    l_map -> setProperties(adj_x, adj_y, false, false);
                }
            }

            //everything else
            else 
            {
                getTileAt(adj_x, adj_y)->updateTileType(TileTypes::FloorTileTypeType); //for floor
                getTileAt(adj_x, adj_y)->tile->representation->setFGColor(*(getTileAt(adj_x, adj_y)->tile->representation->fg_color) * 0.5f, true, false, true); //set darker indoor color
            }

        }
    }



};

int Map::build_from_file(string filename)
{
    string line;
    ifstream myfile (filename);


    if (myfile.is_open())
    {
        // get width
        getline (myfile,line);
        width = atoi(line.c_str());

        // get height
        getline (myfile,line);
        height = atoi(line.c_str());

        l_map = new TCODMap(width, height);
        // printf("Width: %s, Height: %s\n", width, height);
        // cout << width << " " << height << endl;

        // get default tile description
        getline (myfile,line);
        description = line;

        tileArray = new Tile[width*height];

        int i = 0;
        int x = 0;
        int y = 0;
        while (i < width*height) //had to change this from file.good because it was reading too far. Not sure yet.
        {
            getline (myfile,line);

            getline (myfile,line);
            int tileType = atoi(line.c_str());
            tileArray[i].map = this;
            tileArray[i].updateTileType(tileType);

            // printf("x %i y %i\n", x, y);
            if(tileArray[i].type_id == TileTypes::FloorTileTypeType)
            {
                //light passes though, walkable
                l_map -> setProperties(x, y, true, true);

                // printf("see through ");
                // printf("this should be true: %s\n", BoolToString(l_map->isWalkable(x, y)));
            }

            else 
            {
                l_map -> setProperties(x, y, false, false);
                // printf("NOT see through ");
                // printf("this should be false: %s\n", BoolToString(l_map->isWalkable(x, y)));
            }

            if(tileArray[i].type_id == TileTypes::WarpTileTypeType)
            {
                WarpTileType* warp_tile;
                warp_tile = (WarpTileType*) tileArray[i].tile;

                getline (myfile,line);
                warp_tile->warpMap = atoi(line.c_str());
                getline (myfile,line);
                warp_tile->warpX = atoi(line.c_str());
                getline (myfile,line);
                warp_tile->warpY = atoi(line.c_str());

                getline (myfile,line);
                warp_tile->description = line;
            }
            else
            {
                getline (myfile,line);
                tileArray[i].tile->description = line;
            }

            tileArray[i].tile_x = x;
            tileArray[i].tile_y = y;

            // printf("x: %i, y: %i\n", x, y);
            if ( x >= (width -1)  ) // width is 1, only tile would be (0, 0) so you need to substract 1
            {
                y++;
                x = 0;
            }
            else 
            {
                x++;

            };

            i++;
        }

        myfile.close();
    }

    return 1;
}

int Map::draw()
{
    int x,y;

    l_map->computeFov(Game::player->x, Game::player->y, 10, true, FOV_SHADOW);

    for(x=0; x<width;x++)
    {
        for(y=0; y<height; y++)
        {
            Tile * the_tile = getTileAt(x, y);
            TCODColor* the_bg_color = &(TCODColor)(TCODColor::black);
            TCODColor* the_fg_color = &(TCODColor)(TCODColor::white);
            char the_char;

            // cout << "initial white and black" << endl;
            // cout << the_fg_color << endl << the_bg_color << endl;

            if (Game::debug_opts->all_vision || l_map->isInFov(x, y))
                // if (true)
            {
                the_tile->setKnown(true);

                if(the_tile->is_occupied())
                {
                    //check for item first, so it'll get drawn over by actors
                    if (the_tile->check_for_items())
                    {
                        // Game::game_console->putChar(x, y, the_tile->inventory->items->back()->repr->repr);
                        the_char = the_tile->inventory->items->back()->repr->repr;
                    };
                    Game::player->ActorInSight(x, y, the_tile->occupant);

                    the_char = the_tile->occupant->representation->repr;
                    the_fg_color = the_tile->occupant->representation->fg_color;
                }
                //tile is not occupied
                else
                {
                    the_char = the_tile->tile->representation->repr;

                    TCODColor* tile_temp_col = the_tile->tile->representation->temp_bg_color;
                    TCODColor* tile_orig_col = the_tile->tile->representation->bg_color;

                    Game::game_console->putChar(x, y, the_char);
                    //if the temp colour isnt the original color
                    if ( tile_temp_col != tile_orig_col) {
                        the_bg_color = tile_temp_col;
                        the_tile->tile->representation->temp_bg_color = tile_orig_col;
                    }
                    //if the two colors are the same
                    else 
                    {
                        the_fg_color = the_tile->tile->representation->fg_color;
                    };

                    if (the_tile->check_for_items())
                    {
                        // Game::game_console->putChar(x, y, the_tile->inventory->items->back()->repr->repr);
                        Item* back_item =the_tile->inventory->items->back(); 
                        the_char = back_item->repr->repr;
                        the_fg_color = back_item->repr->fg_color;
                    };
                };

                if (l_map->isTransparent(x, y) == true) 
                {
                }
                else
                {
                };

                Game::game_console->putChar(x, y, the_char);
                Game::game_console->setCharForeground(x, y, *the_fg_color);
                Game::game_console->setCharBackground(x, y, *the_bg_color);


            }
            //tile isn't in FOV
            else {

                if (the_tile->is_known() == true)
                {
                    char the_char = the_tile->tile->representation->repr;
                    Game::game_console->putChar(x, y, the_char);
                    Game::game_console->setCharBackground(x, y, *(the_tile->tile->representation->bg_color) * TCODColor::darkGrey);
                    Game::game_console->setCharForeground(x, y, *(the_tile->tile->representation->fg_color) * TCODColor::darkGrey);
                }
                else {
                    Game::game_console->setCharBackground(x, y, TCODColor::black);
                    Game::game_console->setCharForeground(x, y, TCODColor::black);
                }
            };

            //TODO Debug walkable
            //  if (the_tile->map->l_map->isWalkable(the_tile->tile_x, the_tile->tile_y) == false)
            // {
            //              TCODConsole::root->setCharBackground(x, y, TCODColor::amber);
            // }
            // cout << "final white and black" << endl;
            // cout << the_fg_color << endl << the_bg_color << endl << endl;
        }
    }

    //TODO debug pathing
    // std::vector<Actor*>* ais = Game::player->actors_in_sight;
    // for(std::vector<Actor*>::iterator it = ais->begin(); it != ais->end(); ++it) {
    //     char the_char = (*it)->representation->repr;
    //     TCODConsole::root->putChar((*it)->dest_x, (*it)->dest_y,the_char);
    //     TCODConsole::root->setCharForeground((*it)->dest_x, (*it)->dest_y, TCODColor::darkRed);

    //     if ((*it)->l_path != NULL){
    //         for (int i=0; i < (*it)->l_path->size(); i++ ) {
    //             int path_x,path_y;
    //             (*it)->l_path->get(i,&path_x,&path_y);
    //             TCODConsole::root->putChar(path_x, path_y, 'x');
    //             TCODConsole::root->setCharBackground(path_x, path_y, TCODColor::lightRed);
    //             // printf ("Astar coord : %d %d\n", path_x,path_y );
    //         }
    //     }
    //     else {
    //         cout << "its null" << endl;
    //     }
    // }

    //may have just shot readability in the head here...
    // cout << endl << endl;
    // cout << "Tile Description:" << endl;

    Person  * thePerson = Game::player;
    //BaseTileType * person_tile = tileArray[thePerson->x+(thePerson->y*width)].tile;
    BaseTileType * person_tile = getTileAt(thePerson->x, thePerson->y)->tile;


    string pers_desc = person_tile->description;
    string tile_description = (pers_desc != "none" ?  pers_desc : description);
    // cout << tile_description;
    // cout << endl << endl;

    //draw the game_console to root, taking from where the camera is looking at
    TCODConsole::root->blit(Game::game_console, Game::camera_x, Game::camera_y,
            Game::camera_w, Game::camera_h, TCODConsole::root, 0, 0);

    return 1;
}

bool Map::attackMovePlayer(Person *thePerson, int x2, int y2)
{
    // cout << "trying to move player" << endl;

    int new_x, new_y; // where the player is intending to go
    new_x = thePerson->x+x2;
    new_y = thePerson->y+y2;

    //check to make sure the target tile's position would be valid
    if(new_x >= width || new_x < 0 || new_y >= height || new_y < 0)
    {
        cout << "invalid move fool" << endl;
        new Message(Ui::msg_handler_main, "Leave the outer limits alone");
        return false;
    }

    Tile *player_tile; // the current player position
    player_tile = getTileAt(thePerson->x, thePerson->y);

    Tile *target_tile; // the tile of the new position
    target_tile = getTileAt(new_x, new_y);

    std::stringstream converter;
    converter << target_tile->is_occupied();
    string bool_string = converter.str();

    int vec[] = {3, 2, 5, 6}; //walkable types of tiles
    std::vector<int> vec_ints(vec, vec+4);
    bool in_types = std::find(vec_ints.begin(), vec_ints.end(), target_tile->type_id)!=vec_ints.end();

    if (new_x < width && new_x > -1 && new_y < height && new_y > -1 &&
            (in_types) && !target_tile->is_occupied())
    {
        thePerson->has_attacked = false;
        thePerson->putPerson(target_tile, new_x, new_y);
        return true;
    }

    //fight if the tile is occupied
    else if (target_tile->is_occupied())
    {
        bool is_fighter = target_tile->occupant->is_fighter;
        if (is_fighter)
        {
            thePerson->has_attacked = true;
            thePerson->attack(target_tile->occupant);
            return false;
        }
        else 
        {
            thePerson->has_attacked = false;
        };
    }

    //doors
    else if (target_tile->type_id == TileTypes::DoorTileTypeType)
    {
        if (((DoorTileType*)target_tile->tile)->is_open)
        {
            thePerson->putPerson(target_tile, new_x, new_y);
        }
        else
        {
            std::cout << "that door's closed bro." << std::endl;
        };
    }

    else
    {
        cout << endl << "invalid move" << endl;
        new Message(Ui::msg_handler_main, "That's probably a wall.");
        if(new_x < width && new_x > -1 && new_y < height && new_y > -1)
        {
            cout << target_tile->tile->description << endl;
        }
        else
        {  // more blank space for gui consistency
            cout << endl;
        }

        return false;
    }
    return false;
}
