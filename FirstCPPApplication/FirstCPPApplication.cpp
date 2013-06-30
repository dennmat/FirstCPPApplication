#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

#include "utils.h"
#include "Person.h"
#include "Pet.h"
#include "FirstCPPApplication.h"



void WelcomeMessage(){

    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    // printf("\n\n");
    printf("\n\t\t   Prepare to die!\n\n");

};

int main ()
{
    WelcomeMessage();

    //init player
    Person player1;
    player1.name = "Josh";
    player1.age = 23;

    Pet p1_pet;
    p1_pet.master = &player1;

    //init enemy
    Pet enemy_pet;
    Person enemy_player;
    enemy_pet.master = &enemy_player;
    enemy_player.name = "Max";
    enemy_player.age = 50;


    //do BATTLE one way

    bool battle_done = false;
    while (!battle_done){

        std::string answer = ask_for_str("Fight?\n");
        answer = ToLower(answer);

        if (answer == "n" || answer == "no"){
            battle_done = true;
            printf("Fine be that way\n");
            break;
        }

        int damage  = ask_for_int("how much damage do you deal?\n");
        p1_pet.Attack(&enemy_pet, damage);

    }

    //attack player


    std::cout << "Hit enter to exit" << std::endl;
    // std::cin.ignore();
    std::cin.get();

    return 0;
}
