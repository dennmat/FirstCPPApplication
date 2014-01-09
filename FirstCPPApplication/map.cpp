#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include "libtcod.hpp"

#include "map.h"
#include "Room.h"
#include <actors\Person.h>
#include "game.h"
#include "item.h"
#include "inventory.h"
#include "Representation.h"
#include "tile.h"


using namespace std;

Map::Map()
{

    this->roomVector = new vector<Room*>;

}

Map::~Map()
{

}

Tile * Map::getTileAt(int x, int y, bool is_original_pos, int ox, int oy)
{
    vector<Tile> * temp;

    if (!is_original_pos && (x < 0 || y < 0 ) )
    { 
        cout << "can't find a tile here.";
        cout << "I'd throw and error but I don't know how yet" << endl;
    }

    // these two ifs cut way the hell down on recursion by eliminating the
    // need entirely I think
    if (x > this->width)
    {
        x = this->width-1;
    };

    if (y > this->height)
    {
        y = this->height-1;
    };


    //try to get the y vector
    try { temp = &tileVector->at(y); }
    catch ( std::out_of_range& ex )
    {
        ex;
	std::cout << "recursed" << std::endl;
        return getTileAt(x, y-1, false, x, y);
    };

    //try to get the tile from the x vector in the y vector
    // and sense an message if the position isn't the same as the original position
    try {
        temp->at(x);
        // if (!is_original_pos)
        // {
        //     cout << "original position was " << ox << " and ";
        //     cout << oy << " but setting to " << x << " and " << y;
        //     cout << " instead." << endl;
        // }
        return &(*tileVector)[y][x]; 
    }
    catch ( std::out_of_range& ex )
    {
        ex;
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
        BspListener(Map &map) : map(map), roomNum(0) {};
        bool visitNode(TCODBsp *node, void *userData)
        {
            if (node->isLeaf())
            {
                std::cout << "nodes a leaf" << std::endl;

                int x,y,w,h;
                // dig a room
                TCODRandom *rng=TCODRandom::getInstance();
                w=rng->getInt(0, node->w-2);
                h=rng->getInt(0, node->h-2);
                x=rng->getInt(node->x+1, node->x+node->w-w-1);
                y=rng->getInt(node->y+1, node->y+node->h-h-1);
                //map.createRoom(roomNum == 0, x, y, x+w-1, y+h-1);
                //((Map*)userData)->build_rect_room(x, y, x+w-1, y+h-1, 0);
                map.build_rect_room(x, y, x+w-1, y+h-1, 5);
                //Room(x, y, x+w-1, y+h-1);
                // if ( roomNum != 0 ) {
                //     //dig a corridor from last room
                //         map.dig(lastx,lasty,x+w/2,lasty);
                //     map.dig(x+w/2,lasty,x+w/2,y+h/2);
                // }

                lastx=x+w/2;
                lasty=y+h/2;
                roomNum++;
            }
            else
            {
            std::cout << "nodes NOT A  leaf" << std::endl;
            }
            return true;
        };
};

int Map::build_from_random(int seed)
{
    width = 60;
    height = 40;
    l_map = new TCODMap(width, height);
    //the default tile description
    description = "poppycock";

    // tileArray = new Tile[width*height];
    tileVector = new vector<vector<Tile>>;
    tileVector->resize(height);
    for(int ix = 0; ix < height; ++ix)
    {
        (*tileVector)[ix].resize(width);
    }

    int i = 0;
    int x = 0;
    int y = 0;

    int room_max_x = 16;
    int room_max_y = 18;

    while ( i < width*height )
    {
        Tile* this_tile = getTileAt(x, y);
        this_tile->map = this;
        this_tile->updateTileType(3);
        if(this_tile->type_id == 3)
        {
            //light passes though, walkable
            l_map -> setProperties(x, y, true, true);

            // printf("see through ");
            // printf("this should be true: %s\n", BoolToString(l_map->isWalkable(x, y)));
        }

        else 
        {
            //light does NOT pass through nor is walkable
            l_map -> setProperties(x, y, false, false);
            // printf("NOT see through ");
            // printf("this should be false: %s\n", BoolToString(l_map->isWalkable(x, y)));
        }

        this_tile->tile->description = "another desc";

        this_tile->tile_x = x;
        this_tile->tile_y = y;

        // printf("x: %i, y: %i\n", x, y);
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

    // build_rect_room(10, 10, 10, 10, 3);
    // build_rect_room(50, 30, 5, 10, 5);
    // build_rect_room(0, 5, 5, 3, 7);


    TCODBsp bsp(0, 0, width, height);
    bsp.splitRecursive(NULL, 8, room_max_x, room_max_y, 1.5f, 1.5f);
    BspListener listener(*this);
    bsp.traverseInvertedLevelOrder(&listener, this);


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
                adj_y = this->height-1;
            }

            //check for outer perimeter
            if (room->isPerimeter(new_x, new_y))
            {
                getTileAt(adj_x, adj_y)->updateTileType(1);
                l_map -> setProperties(adj_x, adj_y, false, false);

                //place door if valid position
                // room->_current_door_number++;
                // if (room->_current_door_number == room->_door_number_limit )
                // if (new_x == 3 && new_y == 0)
                if (room->checkDoorCount())
                {
                    getTileAt(adj_x, adj_y)->updateTileType(4);
                    l_map -> setProperties(adj_x, adj_y, false, false);
                }
            }

            //everything else
            else 
            {
                getTileAt(adj_x, adj_y)->updateTileType(2);
                l_map -> setProperties(adj_x, adj_y, false, false);
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
            if(tileArray[i].type_id == 3)
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

            if(tileArray[i].type_id == 2)
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

int Map::draw(Game *theGame)
{
    int x,y;

    l_map->computeFov(theGame->player->x, theGame->player->y, 10, true, FOV_SHADOW);

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

            if (l_map->isInFov(x, y))
            {
                the_tile->setKnown(true);

                if(the_tile->is_occupied())
                {
                    //check for item first, so it'll get drawn over by actors
                    if (the_tile->check_for_items())
                    {
                        // TCODConsole::root->putChar(x, y, the_tile->inventory->items->back()->repr->repr);
                        the_char = the_tile->inventory->items->back()->repr->repr;
                    };
                    theGame->player->ActorInSight(x, y, the_tile->occupant);

                    the_char = the_tile->occupant->representation->repr;
                    the_fg_color = the_tile->occupant->representation->fg_color;
                }
                //tile is not occupied
                else
                {
                    the_char = the_tile->tile->representation->repr;

                    TCODColor* tile_temp_col = the_tile->tile->representation->temp_bg_color;
                    TCODColor* tile_orig_col = the_tile->tile->representation->bg_color;

                    TCODConsole::root->putChar(x, y, the_char);
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
                        // TCODConsole::root->putChar(x, y, the_tile->inventory->items->back()->repr->repr);
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

                TCODConsole::root->putChar(x, y, the_char);
                TCODConsole::root->setCharForeground(x, y, *the_fg_color);
                TCODConsole::root->setCharBackground(x, y, *the_bg_color);


            }
            //tile isn't in FOV
            else {

                if (the_tile->is_known() == true)
                {
                    char the_char = the_tile->tile->representation->repr;
                    TCODConsole::root->putChar(x, y, the_char);
                    TCODConsole::root->setCharBackground(x, y, *(the_tile->tile->representation->bg_color) * TCODColor::darkGrey);
                    TCODConsole::root->setCharForeground(x, y, *(the_tile->tile->representation->fg_color) * TCODColor::darkGrey);
                }
                else {
                    TCODConsole::root->setCharBackground(x, y, TCODColor::black);
                    TCODConsole::root->setCharForeground(x, y, TCODColor::black);
                }
            };

            //TODO Debug walkable
            // if (the_tile->map->l_map->isWalkable(the_tile->tile_x, the_tile->tile_y) == false)
            {
                //         TCODConsole::root->setCharBackground(x, y, TCODColor::amber);
            }
            // cout << "final white and black" << endl;
            // cout << the_fg_color << endl << the_bg_color << endl << endl;
        }
    }

    //TODO debug pathing
    // std::vector<Actor*>* ais = theGame->player->actors_in_sight;
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

    Person  * thePerson = theGame->player;
    //BaseTileType * person_tile = tileArray[thePerson->x+(thePerson->y*width)].tile;
    BaseTileType * person_tile = getTileAt(thePerson->x, thePerson->y)->tile;


    string pers_desc = person_tile->description;
    string tile_description = (pers_desc != "none" ?  pers_desc : description);
    // cout << tile_description;
    // cout << endl << endl;



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
        cout << "invalid move" << endl;
        return false;
    }

    Tile *player_tile; // the current player position
    player_tile = getTileAt(thePerson->x, thePerson->y);

    Tile *target_tile; // the tile of the new position
    target_tile = getTileAt(new_x, new_y);

    std::stringstream converter;
    converter << target_tile->is_occupied();
    string bool_string = converter.str();

    if(new_x < width && new_x > -1 &&
            new_y < height && new_y > -1 &&
            (target_tile->type_id == 3 || target_tile->type_id == 2) &&
            !target_tile->is_occupied())
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
    else if (target_tile->type_id == 4)
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
