#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <Windows.h>

#include "libtcod.hpp"

#include "game.h"

#include <actors/person.h>
#include "map.h"
#include "FirstCPPApplication.h"
#include "utils.h"
#include "Representation.h"
#include "build.h"
#include "input.h"
#include "map.h"
#include "thinker.h"
#include "item.h"
#include "inventory.h"
#include "tile.h"	
#include "combat.h"
#include "ui.h"
#include "attribute_container.h"
#include "attribute.h"
#include "enemies\troll.h"
#include "enemies\skeleton.h"
#include <enums\gamestate.h>
#include "item_effect.h"
#include "Room.h"
#include "debug_options.h"
#include "messages.h"

// Game initialization
DebugOptions* Game::debug_opts = new DebugOptions;

int Game::screen_w = 80; //the average RL resolution
int Game::screen_h = 50;

int Game::map_width = 160;
int Game::map_height = 140;

int Game::view_width = 60;
int Game::view_height = 40;

int Game::camera_w = Game::view_width;
int Game::camera_h = Game::view_height;
int Game::camera_x = 0;
int Game::camera_y = 0;

int Game::enemies_size = 255; //hardcoded
bool Game::buildmode = false;
int Game::fps_limit= 120; //default
TCODConsole* Game::game_console = new TCODConsole(Game::map_width, Game::map_height);

std::string Game::last_cmd = "not set";

GameStates Game::current_state = GameStates::GameplayState;

Person* Game::player = NULL;     //the PC

TCOD_key_t Game::key_evt;
TCOD_mouse_t Game::mouse_evt ;

std::vector<Actor*> Game::enemies = std::vector<Actor*>();  //later, this will be an array of all the enemies 
std::vector<Item*> Game::items = std::vector<Item*>();  //later, this will be an array of all the enemies 

unsigned long int Game::turn_count = 1;
unsigned long long int Game::tick_count = NULL;

Map* Game::world = NULL;
Map* Game::current_map = NULL;
int Game::current_map_index = NULL;
Tile* Game::clipboard = NULL;




Map* Game:: buildworld()
{

    cout << get_exe_path() << endl;

    string line;
    ifstream myfile (get_data_path()+"world.txt");
    int num_of_worlds;

    if (myfile.is_open())
    {
        // get width
        getline (myfile,line);
        num_of_worlds = atoi(line.c_str());

        world = new Map;
        //world->the_game = this;
        world[0].build_from_random(0);
    }
    else
    {
        TCHAR exepath[MAX_PATH+1];
        GetModuleFileName(0, exepath, MAX_PATH+1);
        MessageBox(0, exepath, _T("ERROR"), MB_OK);
        MessageBox(0, _T("File used to buildworld not found"), _T("ERROR"), MB_OK);
        exit(EXIT_FAILURE);
    };

    // Game::current_map = world;

    bool is_troll = true;
    for (std::vector<Room*>::iterator it = Game::world->roomVector->begin(); it != Game::world->roomVector->end(); ++it)
    {
        //place a troll in the middle of the room
        int troll_x, troll_y;
        TCODRandom *rng = TCODRandom::getInstance();
        int enemy_count = rng->getInt(1, 5);
        if (is_troll)
        {

            for (int i = 0; i <= enemy_count; i++)
            {
                troll_x = rng->getInt(1, (*it)->width-2) + (*it)->x;
                troll_y = rng->getInt(1, (*it)->height-2) + (*it)->y;
                Game::enemies.push_back( Game::create_troll("Random Troll", 34, troll_x, troll_y, 'T', world, "troll combat"));
                is_troll = false;
            }
        }
        else
        {
            for (int i = 0; i <= enemy_count; i++)
            {
                troll_x = rng->getInt(1, (*it)->width-2) + (*it)->x;
                troll_y = rng->getInt(1, (*it)->height-2) + (*it)->y;
                Game::enemies.push_back(Game::create_skeleton("Random Skeleton", 92, troll_x, troll_y, 's', world, "skeleton combat"));
                is_troll = true;
            }
        }
    }
    return world;
}

//creates a person and places them on the current map
Person * Game::create_person(string name, int age, int x, int y, char repr, 
        Map* map, string Combat_name)
{
    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;
};

Troll * Game::create_troll(string name, int age, int x, int y, char repr, 
        Map* map, string Combat_name)
{
    //build the Person
    Troll * new_pers = new Troll(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

Skeleton * Game::create_skeleton(string name, int age, int x, int y, char repr,
        Map* map, string Combat_name)
{

    //build the Person
    Skeleton * new_pers = new Skeleton(name, age, x, y, repr, Combat_name);


    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

//creates a bunch of items on the map
void  Game::initialize_items(){

    //put it on the map somewhere
    Tile * next_tile = current_map->getTileAt(1,1);

    Item* item = new Item();
    next_tile->place_item_down(item);
    items.push_back(item);

    Item* item2 = new Item();
    item2->name = "Item of Holding";
    item2->description = "You can hold this item";
    player->inventory->add_item(item2);

};

//creates a bunch of enemies on the map
void  Game::initialize_enemies(){

    enemies.push_back(Game::create_person("First", 99, 20, 2, 'p', Game::current_map, "First Person"));
    enemies.push_back(Game::create_troll("Second", 66, 4, 9, 'T', Game::current_map, "Second, Troll"));
    enemies.push_back(Game::create_skeleton("Third", 33, 14, 9, 's', Game::current_map, "Third, Skeleton"));

};

Person*  Game::initialize_player()
{

    player = new Person("Josh", 23, 3, 3, '@', "PLAYER ONE");
    // player->representation->fg_color = &(TCODColor)(TCODColor::celadon);
    player->representation->setFGColor(TCODColor::celadon, true, true, true);
    player->attrs->damage->current_val=9999;
    delete player->thinker;
    player->thinker = NULL;

    Tile* next_tile = Game::current_map->getTileAt(player->x, player->y);
    player->putPerson(next_tile, player->x, player->y);

    return player;

};

// Game::Game()
// {
//     // ui = new Ui(this);
//     Ui::game = this;
//     Game::debug_opts = new DebugOptions;
// 
//     screen_w = 80; //the average RL resolution
//     screen_h = 50;
// 
//     enemies_size = 255; //hardcoded
//     buildmode = false;
// 
//     fps_limit= 120; //default
// 
//     buildworld();
//     current_map = world; //I'm not so sure about this, but it solved the mem issue
// 
//     initialize_player(); //created the Person player
//     initialize_enemies(); // create the enemies
//     initialize_items(); // create the items
// 
//     last_cmd = "not set";
// 
//     Game::current_state = GameStates::GameplayState;
// 
// };

void Game::update()
{

    Game::player->update();
    //explode while level 2
    // if (Game::player->level == 2)
    // {
    //     Tile* tile = Game::player->my_tile;

    //     std::vector<Tile*>* adj_tiles = tile->getAdjacentTiles(2);
    //     for (std::vector<Tile*>::iterator it = adj_tiles->begin(); it != adj_tiles->end(); ++it)
    //     {
    //         (*it)->tile->representation->temp_bg_color = (TCODColor*)(&TCODColor::red);
    //     };
    // }

    for (std::vector<Actor*>::size_type i = 0; i != enemies.size(); i++) 
    {
        Actor* enemy = enemies.at(i);
        // cout << "\t" << enemy->name << "is updating" << endl;
        if (enemy->is_active)
        {
            enemy->update();
        };
        // printf("updating\n");
    }
    // cout << "\t" << "done updating" << endl;
};

void Game::update_ui()
{
    switch(Game::current_state)
    {
        case GameStates::GameplayState: 
            Ui::update_ui();
            break;
        case GameStates::MenuState:
            Ui::update_inventory_ui();
            break;
    };
}

void Game::draw_ui()
{
    switch(Game::current_state)
    {
        case GameStates::GameplayState: 
            Ui::draw_ui();
            break;
        case GameStates::MenuState:
            Ui::draw_inventory_main();
            break;
    };
};

void Game::mainloop()
{

    WelcomeMessage();

    // cout << screen_w << endl;
    // cout << screen_h << endl;
    TCODConsole::initRoot(screen_w, screen_h, "FirstCPPApplication", false);
    TCODConsole::setKeyboardRepeat(500, 1);

    //move main window over a bit so that the console isn't blocked
    move_window(600, 100);


    bool battle_done = false;
    bool incr_turn  = false;
    turn_count = 1;

    Game::fps_limit = 60;

    current_map->draw();
    TCODSystem::setFps(fps_limit);

    new Message(Ui::msg_handler_main, "TURN COUNT %c%d%c", TCOD_COLCTRL_1, Game::turn_count, TCOD_COLCTRL_STOP);
    //

    //draw the map to libtconsole
    current_map->draw();
    //draw the UI
    Game::draw_ui();

    //draw libtcon to screen
    TCODConsole::flush();

    // Game::tick_count = 1;
    bool tick_printed = true;
    while ( !TCODConsole::isWindowClosed() ) {

        TCOD_event_t evt = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key_evt, &mouse_evt);
        switch(Game::current_state)
        {
            case GameStates::GameplayState: 
                if (incr_turn)
                {
                    turn_count++;

                    //this used to be after input was processed but turn hadn't
                    //been incremented
                    int item_count = player->my_tile->inventory->get_count();
                    if (item_count > 0)
                    {
                        std::string msg_str = (item_count == 1) ? "An item is on the ground" : "%d items are on the ground";
                        Message* msg = new Message(Ui::msg_handler_main, msg_str, item_count);
                    }
                    else 
                    {
                        Message* msg = new Message(Ui::msg_handler_main, "Nothing on the ground");
                    }
                        Message* msg = new Message(Ui::msg_handler_main, "%s", player->my_tile->tile->description.c_str());

                    //new Message(Ui::msg_handler_main, "TURN: %d", Game::turn_count);
                    printf("\n-------------[ TURN: %d ]-------------\n", turn_count);
                    incr_turn = false;
                }

                // TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(key_evt, player);
                }
                if (key_evt.pressed == 1)
                {
                    process_debug_event(key_evt, player);
                }

                process_mouse_event(mouse_evt, player);

                //AIs update
                if (incr_turn == true)
                {
                    //std::vector<Actor*>* ais = player->actors_in_sight;
                    // for(std::vector<Actor*>::iterator it = ais->begin(); it != ais->end(); ++it) {
                    // cout << "Actor in sight: " << (*it)->GetNameC() << endl;
                    // }
                    Game::update();
                }

                Game::update_ui();

                //draw the map to libtconsole
                Game::current_map->draw();

                //draw the UI
                Game::draw_ui();

                break;

            case GameStates::MenuState:
                // std::cout << "in menu state" << std::endl;
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(key_evt, player);
                };

                Game::draw_ui();


                // let them choose one to look at 
                break;
        }

        //draw libtcon to screen
        TCODConsole::flush();
        TCODConsole::root->clear();

        // cout << player->attrs->health->current_val << endl;
        //cout << player->combat->cur_hp << endl;

        Game::tick_count++;
        // printf("ticks: %d \r", tick_count);
    }

    std::cout << "Hit enter to exit" << std::endl;
};
