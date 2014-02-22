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
#include "enemies\jackal.h"
#include "enemies\skeleton.h"
#include "enemies\ogre.h"
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


enum SpawnTypes {
    TrollSpawn = 1,
    JackalSpawn,
    SkeletonSpawn,
    OgreSpawn
};

SpawnTypes get_spawn_type()
{
    TCODRandom *rng = TCODRandom::getInstance();
    int dice_roll = rng->getInt(0, 100);
    if (dice_roll <= 30)
        return SpawnTypes::TrollSpawn;
    else if (dice_roll <= 60)
        return SpawnTypes::JackalSpawn;
    else if (dice_roll <= 80)
        return SpawnTypes::SkeletonSpawn;
    else if (dice_roll <= 100)
        return SpawnTypes::OgreSpawn;
};

void Game::fill_world(Map* world)
{
    //fill rooms with enemies and monsters
    // bool is_troll = true;
    for (std::vector<Room*>::iterator it = world->roomVector->begin(); it != world->roomVector->end(); ++it)
    {
        SpawnTypes spawn_type = get_spawn_type();
        int creature_x, creature_y;
        TCODRandom *rng = TCODRandom::getInstance();
        if (spawn_type == SpawnTypes::TrollSpawn)
        {

            int enemy_count = rng->getInt(1, Troll::pack_size);
            for (int i = 0; i <= enemy_count; i++)
            {
                creature_x = rng->getInt(1, (*it)->width-2) + (*it)->x;
                creature_y = rng->getInt(1, (*it)->height-2) + (*it)->y;
                Troll* the_troll;
                if (rng->getInt(1, 100) > 10) 
                {
                    the_troll = Game::create_troll("Random Troll", 34, creature_x, creature_y, 'T', world, "troll combat");
                }
                else //hero
                {
                    the_troll = Game::create_troll("Burly Troll", 34, creature_x, creature_y, 'T', world, "Burly troll combat");
                    the_troll->representation->setFGColor(TCODColor::green+TCODColor::green+TCODColor::darkYellow, true, false, true);
                    the_troll->attrs->health->current_val+=the_troll->attrs->health->current_val;
                    the_troll->attrs->health->max_val+=the_troll->attrs->health->max_val;
                    the_troll->xp_value= floor(the_troll->xp_value*1.5);
                };
                world->enemies.push_back(the_troll);
                // is_troll = false;
            }
        }
        else if (spawn_type == SpawnTypes::JackalSpawn)
        {

            int enemy_count = rng->getInt(1, Jackal::pack_size);
            for (int i = 0; i <= enemy_count; i++)
            {
                creature_x = rng->getInt(1, (*it)->width-2) + (*it)->x;
                creature_y = rng->getInt(1, (*it)->height-2) + (*it)->y;
                Jackal* the_jackal;
                if (rng->getInt(1, 100) > 10) 
                {
                    the_jackal = Game::create_jackal("Random Jackal", 31, creature_x, creature_y, 'j', world, "jackal combat");
                }
                else //hero
                {
                    the_jackal = Game::create_jackal("Burly Jackal", 31, creature_x, creature_y, 'j', world, "jackal combat");
                    the_jackal->representation->setFGColor(TCODColor::sepia+TCODColor::sepia+TCODColor::darkYellow, true, false, true);
                    the_jackal->attrs->health->current_val+=the_jackal->attrs->health->current_val;
                    the_jackal->attrs->health->max_val+=the_jackal->attrs->health->max_val;
                    the_jackal->xp_value= floor(the_jackal->xp_value*1.5);
                };
                world->enemies.push_back(the_jackal);
                // is_jackal = false;
            }
        }
        else if (spawn_type == SpawnTypes::OgreSpawn)
        {
            int enemy_count = rng->getInt(1, Ogre::pack_size);
            for (int i = 0; i <= enemy_count; i++)
            {
                creature_x = rng->getInt(1, (*it)->width-2) + (*it)->x;
                creature_y = rng->getInt(1, (*it)->height-2) + (*it)->y;
                Ogre* the_ogre;
                if (rng->getInt(1, 100) > 10) 
                {
                    the_ogre = Game::create_ogre("Random Ogre", 103, creature_x, creature_y, 'O', world, "ogre combat");
                }
                else
                {
                    the_ogre = Game::create_ogre("Strong Ogre", 103, creature_x, creature_y, 'O', world, "strong ogre combat");
                    the_ogre->representation->setFGColor(TCODColor::white*(TCODColor::darkGrey-TCODColor::darkYellow), true, false, true);
                    the_ogre->attrs->health->current_val+=the_ogre->attrs->health->current_val;
                    the_ogre->attrs->health->max_val+=the_ogre->attrs->health->max_val;
                    the_ogre->attrs->damage->current_val+=the_ogre->attrs->damage->current_val;
                    the_ogre->attrs->damage->max_val+=the_ogre->attrs->damage->max_val;
                    the_ogre->xp_value = floor(the_ogre->xp_value*1.5);
                };
                world->enemies.push_back(the_ogre);
                // is_troll = true;
            }
        }
        else if (spawn_type == SpawnTypes::SkeletonSpawn)
        {
            int enemy_count = rng->getInt(1, Skeleton::pack_size);
            for (int i = 0; i <= enemy_count; i++)
            {
                creature_x = rng->getInt(1, (*it)->width-2) + (*it)->x;
                creature_y = rng->getInt(1, (*it)->height-2) + (*it)->y;
                Skeleton* the_skelly;
                if (rng->getInt(1, 100) > 10) 
                {
                    the_skelly = Game::create_skeleton("Random Skeleton", 92, creature_x, creature_y, 's', world, "skeleton combat");
                }
                else
                {
                    the_skelly = Game::create_skeleton("Strong Skeleton", 92, creature_x, creature_y, 's', world, "strong skeleton combat");
                    the_skelly->representation->setFGColor(TCODColor::white*(TCODColor::darkBlue-TCODColor::darkYellow), true, false, true);
                    the_skelly->attrs->health->current_val+=the_skelly->attrs->health->current_val;
                    the_skelly->attrs->health->max_val+=the_skelly->attrs->health->max_val;
                    the_skelly->attrs->damage->current_val+=the_skelly->attrs->damage->current_val;
                    the_skelly->attrs->damage->max_val+=the_skelly->attrs->damage->max_val;
                    the_skelly->xp_value= the_skelly->xp_value*1.5;
                };
                world->enemies.push_back(the_skelly);
                // is_troll = true;
            }
        }
    }

};


Map* Game:: buildworld()
{

    std::cout << get_exe_path() << std::endl;

    std::string line;
    std::ifstream myfile (get_data_path()+"world.txt");
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

    Game::fill_world(world);
    return world;
}

//creates a person and places them on the current map
Person * Game::create_person(std::string name, int age, int x, int y, char repr, 
        Map* map, std::string Combat_name)
{
    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;
};

Troll * Game::create_troll(std::string name, int age, int x, int y, char repr, 
        Map* map, std::string Combat_name)
{
    //build the Person
    Troll * new_pers = new Troll(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

Jackal * Game::create_jackal(std::string name, int age, int x, int y, char repr, 
        Map* map, std::string Combat_name)
{
    //build the Person
    Jackal * new_pers = new Jackal(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

Ogre * Game::create_ogre(std::string name, int age, int x, int y, char repr,
        Map* map, std::string Combat_name)
{

    //build the Person
    Ogre * new_pers = new Ogre(name, age, x, y, repr, Combat_name);


    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

Skeleton * Game::create_skeleton(std::string name, int age, int x, int y, char repr,
        Map* map, std::string Combat_name)
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

    // Item* item = new Item();
    // next_tile->place_item_down(item);
    // items.push_back(item);

    Item* item2 = new Item();
    item2->name = "Item of Holding";
    item2->item_effect->health_current_val = 1;
    item2->item_effect->mana_current_val = 1;
    item2->item_effect->armor_current_val = 1;
    item2->item_effect->damage_current_val = 1;
    item2->description = "You can hold this item";
    player->inventory->add_item(item2);

};

//creates a bunch of enemies on the map
void  Game::initialize_enemies()
{
    Game::current_map->enemies.push_back(Game::create_person("First", 99, 20, 2, 'p', Game::current_map, "First Person"));
    Game::current_map->enemies.push_back(Game::create_troll("Second", 66, 4, 9, 'T', Game::current_map, "Second, Troll"));
    Game::current_map->enemies.push_back(Game::create_skeleton("Third", 33, 14, 9, 's', Game::current_map, "Third, Skeleton"));
};

Person*  Game::initialize_player()
{

    player = new Person("Josh", 23, 3, 3, '@', "PLAYER ONE");
    // player->representation->fg_color = &(TCODColor)(TCODColor::celadon);
    player->representation->setFGColor(TCODColor::celadon, true, true, true);
    player->attrs->health->current_val=50;
    player->attrs->health->max_val=50;
    player->attrs->damage->current_val=10;
    player->attrs->damage->max_val=10;
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

    int enemies_size = Game::current_map->enemies.size();
    for (std::vector<Actor*>::size_type i = 0; i != enemies_size; i++) 
    {
        Actor* enemy = Game::current_map->enemies.at(i);
        // cout << "\t" << enemy->name << "is updating" << endl;
        if (enemy->is_active && enemy->thinker != NULL)
        {
            enemy->update();
            // printf("updating ai\n");
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

bool gameplay_loop(bool incr_turn)
{

    if (incr_turn)
    {
        Game::turn_count++;

        //this used to be after input was processed but turn hadn't
        //been incremented
        int item_count = Game::player->my_tile->inventory->get_count();
        if (item_count == 1)
        {
            std::string msg_str =  "%s is on the ground.";
            Message* msg = new Message(Ui::msg_handler_main, msg_str, Game::player->my_tile->inventory->items->back()->name.c_str());
        }
        else if (item_count > 1)
        {
            std::string msg_str = "%d items are on the ground.";
            Message* msg = new Message(Ui::msg_handler_main, msg_str, item_count);
        }
        else 
        {
            Message* msg = new Message(Ui::msg_handler_main, "Nothing on the ground.");
        }

        //tile description
        Message* msg = new Message(Ui::msg_handler_main, "%s", Game::player->my_tile->tile->description.c_str());

        //new Message(Ui::msg_handler_main, "TURN: %d", Game::turn_count);
        printf("\n-------------[ TURN: %d ]-------------\n", Game::turn_count);
        incr_turn = false;
    }

    // TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
    if (Game::key_evt.c != NULL && Game::key_evt.pressed == 1 ){
        incr_turn = process_key_event(Game::key_evt, Game::player);
    }
    if (Game::key_evt.pressed == 1)
    {
        process_debug_event(Game::key_evt, Game::player);
    }

    process_mouse_event(Game::mouse_evt, Game::player);

    //AIs update
    if (incr_turn == true)
    {
        Game::update();
    }

    Game::update_ui();

    //draw the map to libtconsole
    Game::current_map->draw();

    //draw the UI
    Game::draw_ui();

    return incr_turn;
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
    Game::turn_count = 1;

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
                incr_turn = gameplay_loop(incr_turn);
                break;

            case GameStates::MenuState:
                // std::cout << "in menu state" << std::endl;
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(key_evt, player);
                };

                Game::draw_ui();
                process_mouse_inv_event();

                // let them choose one to look at 
                break;
        }

        //draw the root console to screen to screen
        TCODConsole::flush();
        TCODConsole::root->clear();

        Game::tick_count++;
        // printf("ticks: %d \r", tick_count);
    }

    std::cout << "Hit enter to exit" << std::endl;
};
