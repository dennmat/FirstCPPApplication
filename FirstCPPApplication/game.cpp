#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>

#include <Windows.h>

#include "libtcod.hpp"

#include "game.h"

#include "Person.h"
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
}

//creates a person and places them on the current map
Person * Game::create_person(string name, int age, int x, int y, char repr, 
        string pet_name)
{

    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr, pet_name);

    if ( !pet_name.empty() )
    {
        new_pers->pet->name = pet_name;
    };


    //put it on the map somewhere
    Tile * next_tile = current_map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    return new_pers;

};

//creates a bunch of items on the map
void  Game::initialize_items(){

    //put it on the map somewhere
    Tile * next_tile = current_map->getTileAt(1,1);
    // new_pers->putPerson(next_tile, x, y);

    Item* item = new Item();
    next_tile->place_item_down(item);
    items.push_back(item);
    Item* item2 = new Item();
    player->inventory->add_item(item2);
    // enemies.push_back(create_person("first", 99, 20, 2, 'a', "first's pet"));
    // enemies.push_back(create_person("second", 66, 4, 9, 'b', "second's pet"));

};

//creates a bunch of enemies on the map
void  Game::initialize_enemies(){

    // enemies.push_back(create_person("first", 99, 20, 2, 'a', "first's pet"));
    // enemies.push_back(create_person("second", 66, 4, 9, 'b', "second's pet"));

};

Person*  Game::initialize_player(){

    player = new Person( "Josh", 23, 3, 3, '@', "");
    player->representation->fg_color = TCODColor::celadon;
    delete player->thinker;
    player->thinker = NULL;

    vector<vector<Tile>> * poop = this->current_map->tileVector;
    Tile * next_tile = &(*poop)[player->x][player->y];
    //Tile * next_tile = &current_map->tileArray[player->x + (player->y*current_map->width)];
    player->putPerson(next_tile, player->x, player->y);

    return player;

};

Game::Game()
{

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

};

void Game::update()
{

    player->update(this);

    for (std::vector<Actor*>::size_type i = 0; i != enemies.size(); i++) 
    {
        Actor* enemy = enemies.at(i);
        cout << "\t" << enemy->name << "is updating" << endl;
        enemy->update(this);
        // printf("updating\n");
    }
    cout << "\t" << "done updating" << endl;
};

void Game::update_ui()
{
}

void Game::draw_ui()
{
    draw_ui_msg();
    draw_ui_sidebar();
};

void Game::draw_ui_sidebar()
{
    ui_sidebar_w = 20;
    ui_sidebar_h = screen_h-ui_msg_h;
    TCODConsole *ui_sidebar_con = new TCODConsole(ui_sidebar_w, ui_sidebar_h);

    //reset ui console to default
    TCODColor ui_sidebar_color(10, 5, 5);
    ui_sidebar_con->setDefaultBackground(ui_sidebar_color);
    ui_sidebar_con->clear();


    //draw the message text
    ui_sidebar_con->print(0, 0, "TURN COUNT %d", turn_count);

    //player stats
    ui_sidebar_con->print(0, 2, "PLAYER NAME %s", player->GetNameC());
    ui_sidebar_con->print(0, 3, "PLAYER HP %d", player->pet->cur_hp);

    //mouse stats
    ui_sidebar_con->print(0, 5, "MOUSE X Y" );
    ui_sidebar_con->print(0, 6, "%d %d", mouse_evt.cx, mouse_evt.cy);

    //is moving in a direciton
    const char* move_left;
    const char* move_right;
    move_left = BoolToString(player->is_moving_left, false);
    move_right = BoolToString(player->is_moving_right, false);

    ui_sidebar_con->print(1, 8, "%s", BoolToString(player->is_moving_up, false) );
    ui_sidebar_con->print(0, 9, "%s %s", move_left, move_right);
    ui_sidebar_con->print(1, 10, "%s", BoolToString(player->is_moving_down, false));

    //draw player inventory info
    ui_sidebar_con->print(0, 13, "%d", player->inventory->get_count());

    //draw ui console to root
    TCODConsole::blit(ui_sidebar_con, 0, 0, ui_sidebar_w, ui_sidebar_h, TCODConsole::root, screen_w-ui_sidebar_w, 0 );
    delete ui_sidebar_con;
};

void Game::draw_ui_msg()
{
    ui_msg_w = screen_w;
    ui_msg_h = 10;
    TCODConsole *ui_msg_con = new TCODConsole(ui_msg_w, ui_msg_h);

    //reset ui console to default
    TCODColor ui_msg_color(12,12,12);
    ui_msg_con->setDefaultBackground(ui_msg_color);
    ui_msg_con->clear();

    //draw the message text
    ui_msg_con->print(0, 0, "TURN COUNT %d", turn_count);

    //draw ui console to root
    TCODConsole::blit(ui_msg_con, 0, 0, ui_msg_w, ui_msg_h, TCODConsole::root, 0, screen_h-ui_msg_h);
    delete ui_msg_con;
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
    turn_count = 0;

    fps_limit = 60;

    current_map->draw(this);
    TCODSystem::setFps(fps_limit);

    //draw the map to libtconsole
    current_map->draw(this);
    //draw the UI
    draw_ui();

    //draw libtcon to screen
    TCODConsole::flush();

    while ( !TCODConsole::isWindowClosed() ) {
        if (incr_turn)
        {
            turn_count++;
            printf("\n-------------[ TURN: %d ]-------------\n", turn_count);
            incr_turn = false;
        }

        // TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
        TCOD_event_t evt = TCODSystem::checkForEvent(TCOD_EVENT_ANY, &key_evt, &mouse_evt);
        if (key_evt.c != NULL ){
            incr_turn = process_key_event(this, key_evt, player);
        };

        process_mouse_event(this, mouse_evt, player);

        //AIs update
        if (incr_turn == true)
        {
            std::vector<Actor*>* ais = player->actors_in_sight;
            for(std::vector<Actor*>::iterator it = ais->begin(); it != ais->end(); ++it) {
                cout << "Actor in sight: " << (*it)->GetNameC() << endl;
            }
            update();
        }

        //draw the map to libtconsole
        current_map->draw(this);

        //draw the UI
        draw_ui();

        //draw libtcon to screen
        TCODConsole::flush();

    }

    std::cout << "Hit enter to exit" << std::endl;
};
