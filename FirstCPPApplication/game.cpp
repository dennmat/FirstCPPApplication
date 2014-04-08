#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <assert.h>

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
#include <enemies\bad_mother.h>
#include "civilian.h"
#include "enums\screens.h"
#include "spells.h"
#include "class.h"

// Game initialization
DebugOptions* Game::debug_opts = new DebugOptions;

int Game::screen_w = 80; //the average RL resolution
int Game::screen_h = 50;

int Game::map_width = 160;
int Game::map_height = 140;
int Game::town_width = 60;
int Game::town_height = 40;

int Game::view_width = 60;
int Game::view_height = 40;

int Game::camera_w = Game::view_width;
int Game::camera_h = Game::view_height;
int Game::camera_x = 0;
int Game::camera_y = 0;

int Game::fov_radius = 6;
int Game::enemies_size = 255; //hardcoded
bool Game::buildmode = false;
int Game::fps_limit= 120; //default
TCODConsole* Game::game_console = new TCODConsole(Game::map_width, Game::map_height);

std::string Game::last_cmd = "not set";

GameStates Game::current_state = GameStates::GameplayState;
// GameStates Game::current_state = GameStates::InventoryState;
Screens Game::current_screen = Screens::InventoryScreen;

Person* Game::player = NULL;     //the PC

TCOD_key_t Game::key_evt;
TCOD_mouse_t Game::mouse_evt;

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
    OgreSpawn,
    BadMotherSpawn,
    ErrorSpawn
};

SpawnTypes get_spawn_type()
{
    TCODRandom *spawning_rng = TCODRandom::getInstance();
    int dice_roll = spawning_rng->getInt(0, 100);
    if (dice_roll <= 30)
        return SpawnTypes::TrollSpawn;
    else if (dice_roll <= 60)
        return SpawnTypes::JackalSpawn;
    else if (dice_roll <= 80)
        return SpawnTypes::SkeletonSpawn;
    else if (dice_roll <= 95)
        return SpawnTypes::OgreSpawn;
    else if (dice_roll > 95)
        return SpawnTypes::BadMotherSpawn;
    else
        assert(false && "Math is bad.");
        return SpawnTypes::ErrorSpawn;
};

Tile* Game::get_mouse_tile()
{
    Tile* mouse_tile = Game::current_map->getTileAt(Game::mouse_evt.cx+Game::camera_x, Game::mouse_evt.cy+Game::camera_y);
    return mouse_tile;
};

Tile* Game::get_player_tile()
{
    Tile* player_tile = Game::player->my_tile;
    return player_tile;
};

void Game::fill_town(Map* world)
{
    Person* the_townsmen = Game::create_townsmen("Random Townsmen", 30, 10, 10, 't', world, "To be deleted");
    world->enemies.push_back(the_townsmen);
};

void Game::fill_dungeon(Map* world)
{
    //fill rooms with enemies and monsters
    // bool is_troll = true;
    TCODRandom *spawning_rng = TCODRandom::getInstance();
    spawning_rng->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
    TCODRandom* linear_rng = new TCODRandom();
    spawning_rng->setDistribution(TCOD_DISTRIBUTION_LINEAR);
    for (std::vector<Room*>::iterator it = world->roomVector->begin(); it != world->roomVector->end(); ++it)
    {
        SpawnTypes spawn_type = get_spawn_type();
        int creature_x, creature_y;
        if (spawn_type == SpawnTypes::TrollSpawn)
        {

            Troll* the_creature = spawn_creature<Troll>( spawning_rng, linear_rng, *it, "Random Troll", 34, 'T', "troll combat");
        }
        else if (spawn_type == SpawnTypes::JackalSpawn)
        {

            Jackal* the_creature = spawn_creature<Jackal>( spawning_rng, linear_rng, *it, "Random Jackal", 31, 'j', "Jackal combat");
            //int enemy_count = spawning_rng->getInt(1, Jackal::pack_size, 10);
            //for (int i = 0; i <= enemy_count; i++)
            //{
            //    creature_x = linear_rng->getInt(2, (*it)->width-2) + (*it)->x;
            //    creature_y = linear_rng->getInt(2, (*it)->height-2) + (*it)->y;
            //    if (!world->getTileAt(creature_x, creature_y)->is_walkable()) { continue;};
            //    Jackal* the_creature;
            //    the_creature = Game::create_creature<Jackal>("Random Jackal", 31, creature_x, creature_y, 'j', world, "jackal combat");
            //    if (linear_rng->getInt(1, 100) < 10) 
            //    {
            //        the_creature->championize();
            //    };
            //    world->enemies.push_back(the_creature);
            //}
        }
        else if (spawn_type == SpawnTypes::OgreSpawn)
        {
            Ogre* the_creature = spawn_creature<Ogre>( spawning_rng, linear_rng, *it, "Random Ogre", 103, 'O', "Ogre combat");
            //int enemy_count = spawning_rng->getInt(1, Ogre::pack_size, 2);
            //for (int i = 0; i <= enemy_count; i++)
            //{
            //    creature_x = linear_rng->getInt(2, (*it)->width-2) + (*it)->x;
            //    creature_y = linear_rng->getInt(2, (*it)->height-2) + (*it)->y;
            //    if (!world->getTileAt(creature_x, creature_y)->is_walkable()) { continue;};
            //    Ogre* the_creature;
            //    the_creature = Game::create_ogre("Random Ogre", 103, creature_x, creature_y, 'O', world, "ogre combat");
            //    if (linear_rng->getInt(1, 100) < 10) 
            //    {
            //        // the_creature = Game::create_ogre("Strong Ogre", 103, creature_x, creature_y, 'O', world, "strong ogre combat");
            //        the_creature->championize();
            //    };
            //    world->enemies.push_back(the_creature);
            //}
        }
        else if (spawn_type == SpawnTypes::SkeletonSpawn)
        {
            Skeleton* the_creature = spawn_creature<Skeleton>( spawning_rng, linear_rng, *it, "Random Skeleton", 92, 's', "Skeleton combat");
            //int enemy_count = spawning_rng->getInt(1, Skeleton::pack_size, 5);
            //for (int i = 0; i <= enemy_count; i++)
            //{
            //    creature_x = linear_rng->getInt(2, (*it)->width-2) + (*it)->x;
            //    creature_y = linear_rng->getInt(2, (*it)->height-2) + (*it)->y;
            //    Skeleton* the_creature;
            //    if (!world->getTileAt(creature_x, creature_y)->is_walkable()) { continue;};
            //    the_creature = Game::create_skeleton("Random Skeleton", 92, creature_x, creature_y, 's', world, "skeleton combat");
            //    if (linear_rng->getInt(1, 100) < 10) 
            //    {
            //        // the_creature = Game::create_skeleton("Strong Skeleton", 92, creature_x, creature_y, 's', world, "strong skeleton combat");
            //        the_creature->championize();
            //    };
            //    world->enemies.push_back(the_creature);
            //}
        }
        else if (spawn_type == SpawnTypes::BadMotherSpawn)
        {
            BadMother* the_creature = spawn_creature<BadMother>( spawning_rng, linear_rng, *it, "Random BadMother", 92, 'b', "BadMother combat");
        //    int enemy_count = spawning_rng->getInt(1, BadMother::pack_size, 5);
        //    for (int i = 0; i <= enemy_count; i++)
        //    {
        //        creature_x = linear_rng->getInt(2, (*it)->width-2) + (*it)->x;
        //        creature_y = linear_rng->getInt(2, (*it)->height-2) + (*it)->y;
        //        BadMother* the_creature;
        //        if (!world->getTileAt(creature_x, creature_y)->is_walkable()) { continue;};
        //        the_creature = Game::create_bad_mother("Random BadMother", 92, creature_x, creature_y, 'b', world, "bad mother combat");
        //        if (linear_rng->getInt(1, 100) < 10) 
        //        {
        //            // the_creature = Game::create_skeleton("Strong BadMother", 92, creature_x, creature_y, 's', world, "strong skeleton combat");
        //            the_creature->championize();
        //        };
        //        world->enemies.push_back(the_creature);
        //    }
        }
    }

};

template<class T>
T* Game::spawn_creature( TCODRandom* spawning_rng, TCODRandom* linear_rng, Room* room, std::string name, int age, char repr, std::string combat_name)
{
    int enemy_count = spawning_rng->getInt(1, T::pack_size, T::preferred_pack_size);
    for (int i = 0; i <= enemy_count; i++)
    {
        int enemy_count = spawning_rng->getInt(1, Troll::pack_size, 5);
        int creature_x, creature_y;
        creature_x = linear_rng->getInt(2, room->width-3) + room->x;
        creature_y = linear_rng->getInt(2, room->height-3) + room->y;

        if (!world->getTileAt(creature_x, creature_y)->is_walkable()) {continue;};

        T* the_creature = Game::create_creature<T>(name, age, creature_x, creature_y, repr, combat_name);
        if (linear_rng->getInt(1, 100) < 10) 
        {
            the_creature->championize();
        };

        world->enemies.push_back(the_creature);
    }
    return NULL;
};

Map* Game:: build_town()
{
    world = new Map;
    //world->the_game = this;
    world->build_town_from_random(0);
    Game::fill_town(world);
    return world;
}

Map* Game:: build_world()
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
        world->build_dungeon_from_random(0);
    }
    else
    {
        TCHAR exepath[MAX_PATH+1];
        GetModuleFileName(0, exepath, MAX_PATH+1);
        MessageBox(0, exepath, _T("ERROR"), MB_OK);
        MessageBox(0, _T("File used to buildworld not found"), _T("ERROR"), MB_OK);
        exit(EXIT_FAILURE);
    };

    Game::fill_dungeon(world);
    //_CrtMemDumpAllObjectsSince( NULL );

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

//creates a person and places them on the current map
Person * Game::create_townsmen(std::string name, int age, int x, int y, char repr, 
        Map* map, std::string Combat_name)
{
    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    new_pers->thinker->is_dumb = true;
    new_pers->thinker->civilian = new Civilian;

    new_pers->is_fighter = false;
    delete new_pers->combat;

    return new_pers;
};

template<class T>
T* Game::create_creature(std::string name, int age, int x, int y, char repr, std::string Combat_name)
{
    //build the Person
    T * new_pers = new T(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = Game::world->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;
};

// Troll * Game::create_troll(std::string name, int age, int x, int y, char repr, 
//         Map* map, std::string Combat_name)
// {
//     //build the Person
//     Troll * new_pers = new Troll(name, age, x, y, repr, Combat_name);
// 
//     //put it on the map somewhere
//     Tile * next_tile = map->getTileAt(x,y);
//     new_pers->putPerson(next_tile, x, y);
// 
//     return new_pers;
// 
// };
// 
// BadMother * Game::create_bad_mother(std::string name, int age, int x, int y, char repr, 
//         Map* map, std::string Combat_name)
// {
//     //build the Person
//     BadMother * new_pers = new BadMother(name, age, x, y, repr, Combat_name);
// 
//     //put it on the map somewhere
//     Tile * next_tile = map->getTileAt(x,y);
//     new_pers->putPerson(next_tile, x, y);
// 
//     return new_pers;
// 
// };
// 
// Jackal * Game::create_jackal(std::string name, int age, int x, int y, char repr, 
//         Map* map, std::string Combat_name)
// {
//     //build the Person
//     Jackal * new_pers = new Jackal(name, age, x, y, repr, Combat_name);
// 
//     //put it on the map somewhere
//     Tile * next_tile = map->getTileAt(x,y);
//     new_pers->putPerson(next_tile, x, y);
// 
//     return new_pers;
// 
// };
// 
// Ogre * Game::create_ogre(std::string name, int age, int x, int y, char repr,
//         Map* map, std::string Combat_name)
// {
// 
//     //build the Person
//     Ogre * new_pers = new Ogre(name, age, x, y, repr, Combat_name);
// 
// 
//     //put it on the map somewhere
//     Tile * next_tile = map->getTileAt(x,y);
//     new_pers->putPerson(next_tile, x, y);
// 
//     return new_pers;
// 
// };
// 
// Skeleton * Game::create_skeleton(std::string name, int age, int x, int y, char repr,
//         Map* map, std::string Combat_name)
// {
// 
//     //build the Person
//     Skeleton * new_pers = new Skeleton(name, age, x, y, repr, Combat_name);
// 
// 
//     //put it on the map somewhere
//     Tile * next_tile = map->getTileAt(x,y);
//     new_pers->putPerson(next_tile, x, y);
// 
//     return new_pers;
// 
// };

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
    //Game::current_map->enemies.push_back(Game::create_person("First", 99, 20, 2, 'p', Game::current_map, "First Person"));
    //Game::current_map->enemies.push_back(Game::create_troll("Second", 66, 4, 9, 'T', Game::current_map, "Second, Troll"));
    //Game::current_map->enemies.push_back(Game::create_skeleton("Third", 33, 14, 9, 's', Game::current_map, "Third, Skeleton"));
};

Person*  Game::initialize_player()
{
    player = new Person("Josh", 23, 3, 3, '@', "PLAYER ONE");
    // player->representation->fg_color = &(TCODColor)(TCODColor::celadon);
    player->representation->setFGColor(TCODColor::celadon, true, true, true);

    player->attrs->health->current_val=200;
    player->attrs->health->max_val=200;
    player->attrs->damage->current_val=10;
    player->attrs->damage->max_val=10;
    player->attrs->armor->current_val=3;
    player->attrs->armor->max_val=3;

    player->actor_class = new FighterClass;
    player->actor_class->master = player;

    delete player->thinker;
    player->thinker = NULL;

    player->spells->push_back(new Spell);
    player->spells->back()->name = "Fireball";
    player->spells->back()->spell_effect->health_current_val = 10;
    player->spells->back()->mana_cost = 10;
    player->spells->back()->max_range = 10;

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

    //update player
    Game::player->update();

    //update actors in this floor
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
        // case GameStates::MenuState:
        //     Ui::update_inventory_ui();
        //     break;
    };
}

void Game::draw_ui()
{
    if (Game::current_state == GameStates::GameplayState)
    // switch(Game::current_state)
    {
        // case GameStates::GameplayState: 
            Ui::draw_ui();
            // break;
    }
    else
    {
        switch(Game::current_screen)
        {
            case Screens::InventoryScreen:
                Ui::draw_inventory_main();
                break;
            case Screens::MainMenuScreen:
                Ui::draw_main_menu_ui();
                break;
            case Screens::CharacterSheetScreen:
                Ui::draw_char_sheet_ui();
                break;
            case Screens::HelpScreen:
                Ui::draw_help_ui();
                break;
            case Screens::SpellSelectScreen:
                Ui::draw_spell_select_main();
                break;
            case Screens::ClassSelectScreen:
                Ui::draw_class_select_main();
                break;
            default:
                assert(false && "Unknown Screens");
                break;
        };
    };
};

bool gameplay_loop(bool incr_turn)
{

    if (incr_turn)
    {

        //this used to be after input was processed but turn hadn't
        //been incremented
        int item_count = Game::player->my_tile->inventory->get_count();
        if (item_count == 1)
        {
            std::string msg_str =  "%s is on the ground.";
             new Message(Ui::msg_handler_main, ITEM_MSG, msg_str, Game::player->my_tile->inventory->items->back()->name.c_str());
        }
        else if (item_count > 1)
        {
            std::string msg_str = "%d items are on the ground.";
             new Message(Ui::msg_handler_main, ITEM_MSG, msg_str, item_count);
        }
        else 
        {
             new Message(Ui::msg_handler_main, TILE_DESCRIPTION_MSG, "Nothing on the ground.");
        }

        //tile description
        new Message(Ui::msg_handler_main, TILE_DESCRIPTION_MSG, "%s", Game::player->my_tile->get_description().c_str());

        //new Message(Ui::msg_handler_main, NOTYPE_MSG, "TURN: %d", Game::turn_count);
        Game::turn_count++;
        printf("\n-------------[ TURN: %d ]-------------\n", Game::turn_count);
        incr_turn = false;
    }

    // TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
    if (Game::key_evt.c != NULL && Game::key_evt.pressed == 1 ){
        incr_turn = process_key_event(Game::key_evt);
    }
    if (Game::key_evt.pressed == 1)
    {
        process_debug_event(Game::key_evt);
    }

    process_mouse_event(Game::mouse_evt);

    //AIs update
    if (incr_turn == true)
    {
        // int x = 1;
        Game::update();
    }

    Game::update_ui();

    //draw the map to libtconsole
    Game::current_map->draw();

    //draw the UI
    Game::draw_ui();

    return incr_turn;
};

void Game::start_game()
{
    Actor::actor_class_choices->push_back(new FighterClass);
    Actor::actor_class_choices->push_back(new MageClass);
    Actor::actor_class_choices->push_back(new BrawlerClass);
    std::cout << "starting world gen" << std::endl;
    Map* new_map = Game::build_world();
    std::cout << "ending world gen" << std::endl;
     // Map* last_map = Game::build_town();
    Game::current_map = new_map;

    Game::initialize_player(); //created the Person player
    Game::mainloop();

};

void Game::mainloop()
{

    TCODConsole::initRoot(screen_w, screen_h, "FirstCPPApplication", false);
    TCODConsole::setKeyboardRepeat(1000, 1);

    WelcomeMessage();
    //move main window over a bit so that the console isn't blocked
    move_window(600, 100);

    bool incr_turn  = false;
    Game::turn_count = 1;

    Game::fps_limit = 60;

    current_map->draw();
    TCODSystem::setFps(fps_limit);
    //TCODConsole::setFullscreen(true);
    // auto renderer = TCODSystem::getRenderer();
    // TCODSystem::setRenderer(TCOD_renderer_t::TCOD_RENDERER_OPENGL); //risky
    // TCODSystem::setRenderer(TCOD_renderer_t::TCOD_RENDERER_GLSL); // really fast if you can man it
    TCODConsole::setKeyboardRepeat(250, 10);

    new Message(Ui::msg_handler_main, NOTYPE_MSG, "Run from ogres and skeletons, kill troll and jackals!");
    new Message(Ui::msg_handler_main, NOTYPE_MSG, "You're going to die a lot, this isn't Dark Souls 2.");
    //

    //draw the map to libtconsole
    current_map->draw();
    //draw the UI
    Game::draw_ui();

    //draw libtcon to screen
    TCODConsole::flush();

    assert(Game::world != NULL);
    while ( !TCODConsole::isWindowClosed() ) 
    {

        TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key_evt, &mouse_evt);
        switch(Game::current_state)
        {
            case GameStates::GameplayState: 
                incr_turn = gameplay_loop(incr_turn);
                break;

            case GameStates::MenuState:
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(key_evt);
                };

                Game::draw_ui();
                process_mouse_inv_event();

                break;

            case GameStates::InventoryState:
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(key_evt);
                };

                Game::draw_ui();
                process_mouse_inv_event();
                break;

            case GameStates::MagicState:
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(key_evt);
                };

                Game::draw_ui();
                process_mouse_inv_event();
                break;

            default:
                assert(false && "Unknown gamestate");
                break;
        }

        //draw the root console to screen to screen
        TCODConsole::flush();
        TCODConsole::root->clear();

        // assert(Game::tick_count!= -1);
        Game::tick_count++;
    }

    std::cout << "Hit enter to exit" << std::endl;
};
