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

void Game:: buildworld()
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
        world->the_game = this;
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

    this->current_map = world;

    bool is_troll = true;
    for (std::vector<Room*>::iterator it = this->world->roomVector->begin(); it != this->world->roomVector->end(); ++it)
    {
        //place a troll in the middle of the room
        //it-
        int troll_x, troll_y;
        troll_x = ((*it)->width / 2) + (*it)->x; 
        troll_y = ((*it)->height / 2) + (*it)->y;
        if (is_troll)
        {
            Troll* troll = this->create_troll("random troll", 34, troll_x, troll_y, 'T', "troll combat");
            is_troll = false;
            this->enemies.push_back(troll);
        }
        else
        {
            Skeleton* skeleton = this->create_skeleton("random skeleton", 92, troll_x, troll_y, 's', "skeleton combat");
            is_troll = true;
            this->enemies.push_back(skeleton);
        }
    }


}

//creates a person and places them on the current map
Person * Game::create_person(string name, int age, int x, int y, char repr, 
        string Combat_name)
{
    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr, Combat_name);

    //put it on the map somewhere
    Tile * next_tile = current_map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

Troll * Game::create_troll(string name, int age, int x, int y, char repr, 
        string Combat_name)
{

    //build the Person
    Troll * new_pers = new Troll(name, age, x, y, repr, Combat_name);

    // new_pers->representation->fg_color = getRGBFromColor(TCODColor::darkGreen);

    //put it on the map somewhere
    Tile * next_tile = this->current_map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

Skeleton * Game::create_skeleton(string name, int age, int x, int y, char repr, 
        string Combat_name)
{

    //build the Person
    Skeleton * new_pers = new Skeleton(name, age, x, y, repr, Combat_name);


    //put it on the map somewhere
    Tile * next_tile = current_map->getTileAt(x,y);
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

    enemies.push_back(this->create_person("first", 99, 20, 2, 'p', "First Person"));
    enemies.push_back(this->create_troll("second", 66, 4, 9, 'T', "Second, Troll"));
    enemies.push_back(this->create_skeleton("third", 33, 14, 9, 's', "Third, Skeleton"));

};

Person*  Game::initialize_player()
{

    player = new Person("Josh", 23, 3, 3, '@', "PLAYER ONE");
    // player->representation->fg_color = &(TCODColor)(TCODColor::celadon);
    player->representation->setFGColor(TCODColor::celadon, true, true, true);
    delete player->thinker;
    player->thinker = NULL;

    Tile* next_tile = this->current_map->getTileAt(player->x, player->y);
    player->putPerson(next_tile, player->x, player->y);

    return player;

};

Game::Game()
{
    ui = new Ui;
    ui->game = this;
    this->debug_opts = new DebugOptions;

    screen_w = 80; //the average RL resolution
    screen_h = 50;

    enemies_size = 255; //hardcoded
    buildmode = false;

    fps_limit= 120; //default

    buildworld();
    current_map = world; //I'm not so sure about this, but it solved the mem issue

    initialize_player(); //created the Person player
    initialize_enemies(); // create the enemies
    initialize_items(); // create the items

    last_cmd = "not set";

    this->current_state = GameStates::GameplayState;

};

void Game::update()
{

    this->player->update(this);
    //explode while level 2
    // if (this->player->level == 2)
    // {
    //     Tile* tile = this->player->my_tile;

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
            enemy->update(this);
        };
        // printf("updating\n");
    }
    cout << "\t" << "done updating" << endl;
};

void Game::update_ui()
{
    switch(this->current_state)
    {
        case GameStates::GameplayState: 
            this->ui->update_ui();
            break;
        case GameStates::MenuState:
            this->ui->update_inventory_ui();
            break;
    };
}

void Game::draw_ui()
{
    switch(this->current_state)
    {
        case GameStates::GameplayState: 
            this->ui->draw_ui();
            break;
        case GameStates::MenuState:
            this->ui->draw_inventory_main();
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

    this->fps_limit = 60;

    current_map->draw(this);
    TCODSystem::setFps(fps_limit);

    //draw the map to libtconsole
    current_map->draw(this);
    //draw the UI
    this->draw_ui();

    //draw libtcon to screen
    TCODConsole::flush();

    this->tick_count = 1;
    bool tick_printed = true;
    while ( !TCODConsole::isWindowClosed() ) {

        TCOD_event_t evt = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key_evt, &mouse_evt);
        switch(this->current_state)
        {
            case GameStates::GameplayState: 
                if (incr_turn)
                {
                    turn_count++;
                    printf("\n-------------[ TURN: %d ]-------------\n", turn_count);
                    incr_turn = false;
                }

                // TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(this, key_evt, player);
                }
                if (key_evt.pressed == 1)
                {
                    process_debug_event(this, key_evt, player);
                }

                process_mouse_event(this, mouse_evt, player);

                //AIs update
                if (incr_turn == true)
                {
                    //std::vector<Actor*>* ais = player->actors_in_sight;
                    // for(std::vector<Actor*>::iterator it = ais->begin(); it != ais->end(); ++it) {
                    // cout << "Actor in sight: " << (*it)->GetNameC() << endl;
                    // }
                    this->update();
                }

                this->update_ui();

                //draw the map to libtconsole
                this->current_map->draw(this);

                //draw the UI
                this->draw_ui();

                break;

            case GameStates::MenuState:
                // std::cout << "in menu state" << std::endl;
                if (key_evt.c != NULL && key_evt.pressed == 1 ){
                    incr_turn = process_key_event(this, key_evt, player);
                };

                this->draw_ui();


                // let them choose one to look at 
                break;
        }

        //draw libtcon to screen
        TCODConsole::flush();

        // cout << player->attrs->health->current_val << endl;
        //cout << player->combat->cur_hp << endl;

        this->tick_count++;
        // printf("ticks: %d \r", tick_count);
    }

    std::cout << "Hit enter to exit" << std::endl;
};
