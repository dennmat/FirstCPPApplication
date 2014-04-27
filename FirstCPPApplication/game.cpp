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

int Game::map_width = 60;
int Game::map_height = 40;
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

TCODRandom* Game::spawning_rng = new TCODRandom(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
TCODRandom* Game::linear_rng = new TCODRandom(TCOD_DISTRIBUTION_LINEAR);


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
    // TCODRandom *spawning_rng = TCODRandom::getInstance();
    int dice_roll = Game::spawning_rng->getInt(0, 100);
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
    Person* the_townsmen = Game::create_townsmen("Random Townsmen", 30, 10, 10, 't', world);
    world->enemies.push_back(the_townsmen);
};

void Game::fill_generic_room(Room* room)
{
    SpawnTypes spawn_type = get_spawn_type();
    int creature_x, creature_y;
    if (spawn_type == SpawnTypes::TrollSpawn)
    {
        spawn_creature<Troll>(room, "Random Troll", 34, 'T');
    }
    else if (spawn_type == SpawnTypes::JackalSpawn)
    {
        spawn_creature<Jackal>(room, "Random Jackal", 31, 'j');
    }
    else if (spawn_type == SpawnTypes::OgreSpawn)
    {
        spawn_creature<Ogre>(room, "Random Ogre", 103, 'O');
    }
    else if (spawn_type == SpawnTypes::SkeletonSpawn)
    {
        spawn_creature<Skeleton>(room, "Random Skeleton", 92, 's');
    }
    else if (spawn_type == SpawnTypes::BadMotherSpawn)
    {
        spawn_creature<BadMother>(room, "Random BadMother", 92, 'b');
    }
};

void Game::fill_dungeon(Map* world)
{
    //fill rooms with enemies and monsters
    for (std::vector<Room*>::iterator it = world->roomVector->begin(); it != world->roomVector->end(); ++it)
    {
        if (it == world->roomVector->begin())
        {
            //spawn one dude to whom you can sell your shit
            Person* the_townsmen = Game::create_townsmen("Travelling Salesman", 30, 10, 10, 't', world);
        }

        else
        {
            fill_generic_room(*it);
        }
    }

};

    template<class T>
T* Game::spawn_creature(Room* room, std::string name, int age, char repr)
{
    int enemy_count = Game::spawning_rng->getInt(1, T::pack_size, T::preferred_pack_size);
    for (int i = 0; i <= enemy_count; i++)
    {
        int creature_x, creature_y;
        creature_x = Game::linear_rng->getInt(2, room->width-3) + room->x;
        creature_y = Game::linear_rng->getInt(2, room->height-3) + room->y;

        if (!world->getTileAt(creature_x, creature_y)->is_walkable()) {continue;};

        T* the_creature = Game::create_creature<T>(name, age, creature_x, creature_y, repr);
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

Map* Game::build_world()
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
Person * Game::create_person(std::string name, int age, int x, int y, char repr, Map* map)
{
    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr);

    //put it on the map somewhere
    Tile * next_tile = map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;
};

//creates a person and places them on the current map
Person * Game::create_townsmen(std::string name, int age, int x, int y, char repr, 
        Map* map)
{
    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr);

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
T* Game::create_creature(std::string name, int age, int x, int y, char repr)
{
    //build the Person
    T * new_pers = new T(name, age, x, y, repr);

    //put it on the map somewhere
    Tile * next_tile = Game::world->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;
};


//creates a bunch of items on the map
void  Game::initialize_items()
{

    //put it on the map somewhere
    // Tile * next_tile = current_map->getTileAt(1,1);

    // Item* item = new Item();
    // next_tile->place_item_down(item);
    // items.push_back(item);

    // for (int i = 0; i < 52; i++)
    // {
    //     Item* item2 = new Item();
    //     item2->name = "Item of Holding";
    //     item2->name.append(std::to_string((long double)i+1));
    //     item2->item_effect->health_current_val = 1;
    //     item2->item_effect->mana_current_val = 1;
    //     item2->item_effect->armor_current_val = 1;
    //     item2->item_effect->damage_current_val = 900;
    //     item2->item_effect->damage_max_val = 900;
    //     item2->item_effect->duration= 4;
    //     item2->usable = true;
    //     item2->description = "You can hold this item";
    //     player->inventory->add_item(item2);
    // };

};

// //creates a bunch of enemies on the map
// void  Game::initialize_enemies()
// {
//     //Game::current_map->enemies.push_back(Game::create_person("First", 99, 20, 2, 'p', Game::current_map, "First Person"));
//     //Game::current_map->enemies.push_back(Game::create_troll("Second", 66, 4, 9, 'T', Game::current_map, "Second, Troll"));
//     //Game::current_map->enemies.push_back(Game::create_skeleton("Third", 33, 14, 9, 's', Game::current_map, "Third, Skeleton"));
// };

Person*  Game::initialize_player()
{
    player = new Person("Josh", 23, 0, 0, '@');
    // player->representation->fg_color = &(TCODColor)(TCODColor::celadon);
    player->representation->setFGColor(TCODColor::celadon, true, true, true);

    // player->attrs->health->current_val=200;
    // player->attrs->health->max_val=200;
    player->attrs->health->current_val=30;
    player->attrs->health->max_val=30;
    player->attrs->damage->current_val=10;
    player->attrs->damage->max_val=10;
    player->attrs->armor->current_val=3;
    player->attrs->armor->max_val=3;
    player->attrs->hunger->current_val=210;

    player->actor_class = new FighterClass;
    player->actor_class->master = player;

    delete player->thinker;
    player->thinker = NULL;

    player->spells->push_back(new Spell);
    player->spells->back()->name = "Fireball";
    player->spells->back()->spell_effect->health_current_val = -10;
    player->spells->back()->mana_cost = 10;
    player->spells->back()->max_range = 10;

    //Tile* next_tile = Game::current_map->getTileAt(player->x, player->y);
    Room* room = Game::current_map->roomVector->front();
    int x = room->center_x;
    int y = room->center_y;
    Game::player->putPerson(world->getTileAt(x, y), x, y);
    //player->putPerson(next_tile, player->x, player->y);
    Game::initialize_items();

    Game::center_camera_on_player();

    return player;

};

void Game::center_camera_on(int abs_x, int abs_y)
{
    Game::camera_x = abs_x - (Game::game_console->getWidth()/2);
    Game::camera_y = abs_y - (Game::game_console->getHeight()/2);
};

void Game::center_camera_on(Actor* actor)
{
    Game::center_camera_on(actor->x, actor->y);
};

void Game::center_camera_on(Tile* tile)
{
    Game::center_camera_on(tile->tile_x, tile->tile_y);
};

void Game::center_camera_on_player()
{
    Game::center_camera_on(Game::player);
};

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
        incr_turn = Input::process_key_event(Game::key_evt);
    }
    if (Game::key_evt.pressed == 1)
    {
        Input::process_debug_event(Game::key_evt);
    }

    Input::process_mouse_event(Game::mouse_evt);

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
    // Map* new_map = NULL;
    std::cout << "ending world gen" << std::endl;
    // Map* last_map = Game::build_town();
    Game::current_map = new_map;

    Game::initialize_player(); //created the Person player
    Game::mainloop();

};

void Game::mainloop()
{

    TCODConsole::setCustomFont("data/terminal.png");
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
                // if (key_evt.c != NULL && key_evt.pressed == 1 ){
                //     incr_turn = Input::process_key_event(key_evt);
                // };

                // Game::draw_ui();
                // Input::process_mouse_inv_event();

                // break;

            case GameStates::InventoryState:
                // if (key_evt.c != NULL && key_evt.pressed == 1 ){
                //     incr_turn = Input::process_key_event(key_evt);
                // };

                // Game::draw_ui();
                // Input::process_mouse_inv_event();
                // break;

            case GameStates::MagicState:
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = Input::process_key_event(key_evt);
                };

                Game::draw_ui();
                Input::process_mouse_inv_event();
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

        //when the game is moved, the screen goes black for some reason. This
        //redraws the whole screen to make up for it. f8 does it manually.
        if (Game::tick_count % 50 == 0)         TCODConsole::root->setDirty(0, 0, 1000, 1000);
    }

    std::cout << "Hit enter to exit" << std::endl;
};
