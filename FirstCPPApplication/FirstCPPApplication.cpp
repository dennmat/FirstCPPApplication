#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
#include "Person.h"
#include "Pet.h"



void WelcomeMessage(){

    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    // printf("\n\n");
    printf("\n\t\t   Prepare to die!\n\n");

};

std::string ToLower(std::string data){
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
};

std::string ToUpper(std::string data){
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    return data;
};


void print_str_array(std::string msg_list[], int len){

    std::cout << len << std::endl;
    for (int i = 0; i < len; i++){
    std::cout << msg_list[i] << std::endl;
    };
};

int ask_for_int(std::string msg, int default_int=0){

    // std::cout << msg << std::endl;
    std::cout << msg << std::endl << ">>> ";
    std::string str_answer;
    getline(std::cin, str_answer);
    if (str_answer == ""){
    return default_int;
    }
    else {
    int int_answer = atoi(str_answer.c_str());
    return int_answer;
    }
};

std::string ask_for_str(std::string msg, std::string default_str="STRING"){

    std::cout << msg << std::endl << ">>> ";
    std::string str_answer;
    getline(std::cin, str_answer);

    if (str_answer == ""){
    return default_str;
    }
    else {
    return str_answer;
    }
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
