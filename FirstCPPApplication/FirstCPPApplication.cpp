#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>

using namespace std;

class Person{
    public:
    string name;
    int age;
};

class Pet{
    public:
    string name;
    int max_hp;
    int cur_hp;
    Person* master;

    bool is_dead;

    Pet (string name, int max_hp);
    Pet::Pet();
    // Pet::Init();
    void Pet::Attack(Pet* target, int dmg);
    void Pet::TakeDamage(int dmg);
    void printout();
    bool Pet::CheckDeath();
};

void Pet::printout(){

    cout << "\nPET PRINTOUT" << endl;
    cout << "NAME: " << name << endl;
    cout << "MAX HP: " << max_hp << endl;

};

void WelcomeMessage(){

    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    // printf("\n\n");
    printf("\n\t\t   Prepare to die!\n\n");

};

string ToLower(string data){
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    return data;
};

string ToUpper(string data){
	std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    return data;
};

Pet::Pet(){
    name = "Nameless Pet";
    max_hp = 100;
    cur_hp = max_hp;

    master = NULL;
    is_dead = false;
};

Pet::Pet(string name, int max_hp){
    name = name;
    cur_hp = max_hp;
    max_hp = max_hp;

    master = NULL;
    is_dead = false;


};

void Pet::Attack(Pet* ptr_target, int dmg){
    //attacks another pet for dmg damage
    // cout << ptr_target << endl;

    cout << "About to attack " << (*ptr_target).name << endl;
    ptr_target->TakeDamage(dmg);

    bool boo = ptr_target->CheckDeath();
    if (boo){
        printf("AHAAHAHA, you're dead bro, that's got to feel bad.\n");
    };

};

void Pet::TakeDamage(int dmg){
    printf("About to take %d damage! ", dmg);
    printf("With %d hp.\n", cur_hp);
    cur_hp-=dmg;
    cout << name << " at " << cur_hp << " health left!" << endl;

    CheckDeath();

};

bool Pet::CheckDeath(){
    if (cur_hp <= 0  && !is_dead){
	printf("OH NO I\"M DEAD\n");
	is_dead = true;
    }
    return is_dead;

};

void print_str_array(string msg_list[], int len){

    cout << len << endl;
    for (int i = 0; i < len; i++){
    cout << msg_list[i] << endl;
    };
};

int ask_for_int(string msg, int default_int=0){

    // cout << msg << endl;
    cout << msg << endl << ">>> ";
    string str_answer;
    getline(cin, str_answer);
    if (str_answer == ""){
    return default_int;
    }
    else {
    int int_answer = atoi(str_answer.c_str());
    return int_answer;
    }
};

string ask_for_str(string msg, string default_str="STRING"){

    cout << msg << endl << ">>> ";
    string str_answer;
    getline(cin, str_answer);

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

        string answer = ask_for_str("Fight?\n");
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


    cout << "Hit enter to exit" << endl;
    // cin.ignore();
    cin.get();

    return 0;
}
