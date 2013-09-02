#include "stdafx.h"
#include "game.h"
#include "Person.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "map.h"
#include "FirstCPPApplication.h"
#include "utils.h"
#include "Representation.h"
#include "build.h"

#include "libtcod.hpp"


void Game:: buildworld()
{
    string line;
    ifstream myfile ("world.txt");
    int num_of_worlds;
    int i;

    if (myfile.is_open())
    {
        // get width
        getline (myfile,line);
        num_of_worlds = atoi(line.c_str());


        world = new Map;
        world[0].build_from_random(0);
        // 
        //          world = new Map[num_of_worlds];
        //          for(i=0;i<num_of_worlds;i++)
        //          {
        //              // get height
        //              getline (myfile,line);
        //              world[i].build_from_file(line);
        //          }
    }
}

//creates a person and places them on the current map
Person * Game::create_person(string name, int age, int x, int y, char repr, 
        string pet_name)
{
    ////set its representation, aka color and char
    //Representation * representation =   new Representation;
    //representation->repr = repr;
    // new_pers->representation = representation;

    //build the Person
    Person * new_pers = new Person(name, age, x, y, repr, pet_name);

    if ( !pet_name.empty() )
    {
        new_pers->pet->name = pet_name;
    };


    //put it on the map somewhere
    Tile * next_tile = current_map->getTileAt(x,y);
    new_pers->putPerson(next_tile, x, y);

    // cout << "created person " << name << endl;

    return new_pers;

};

//creates a bunch of enemies on the map
void  Game::initialize_enemies(){

    enemies.push_back(create_person("first", 99, 200, 2, 'a', "first's pet"));
    enemies.push_back(create_person("second", 66, 4, 4, 'b', "second's pet"));

};

Person*  Game::initialize_player(){

    player = new Person( "Josh", 23, 3, 3, '@', "");
    player->representation->color = TCODColor::celadon;

    //Pet p1_pet;
    //p1_pet.master = &player;

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

    last_cmd = "not set";

};

void Game::update()
{

    for (std::vector<Actor*>::size_type i = 0; i != enemies.size(); i++) 
    {
        Actor* enemy = enemies.at(i);
        enemy->update();
        // printf("updating\n");
    }
};

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

    ui_sidebar_con->print(0, 2, "PLAYER NAME %s", player->GetNameC());
    ui_sidebar_con->print(0, 3, "PLAYER HP %d", player->pet->cur_hp);

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

    cout << screen_w << endl;
    cout << screen_h << endl;
    TCODConsole::initRoot(screen_w, screen_h, "FirstCPPApplication", false);
    TCODConsole::setKeyboardRepeat(400, 1);

    bool battle_done = false;
    bool incr_turn  = true;
    turn_count = 1;

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
        if (incr_turn == true)
        {
            printf("\n-------------[ TURN: %d ]-------------\n", turn_count);
        }

        //player input
        TCOD_key_t key_evt;
        TCOD_mouse_t mouse_evt;
        TCOD_event_t evt = TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &key_evt, &mouse_evt, false);
        incr_turn = process_event(key_evt, player);

        //AIs update
        update();

        //draw the map to libtconsole
        current_map->draw(this);
        //draw the UI
        draw_ui();

        //draw libtcon to screen
        TCODConsole::flush();

        if (incr_turn == true)
        {
            turn_count++;
        }

    }

    std::cout << "Hit enter to exit" << std::endl;
};
