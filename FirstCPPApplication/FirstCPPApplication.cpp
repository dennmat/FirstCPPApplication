#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <cstdlib>

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

    Pet (string name, int max_hp);
    Pet::Pet();
    void Pet::Attack(Pet* target, int dmg);
    void Pet::TakeDamage(int dmg);
    void printout();
};

void Pet::printout(){

    cout << "\nPET PRINTOUT" << endl;
    cout << "NAME: " << name << endl;
    cout << "MAX HP: " << max_hp << endl;

};

Pet::Pet(){
    name = "Unamed Pet";
    max_hp = 100;
    cur_hp = max_hp;

    master = NULL;
};

Pet::Pet(string name, int max_hp){
    name = name;
    cur_hp = max_hp;
    max_hp = max_hp;

    master = NULL;

};

void Pet::Attack(Pet* ptr_target, int dmg){
    //attacks another pet for dmg damage
    // cout << ptr_target << endl;

    cout << "About to attack " << (*ptr_target).name << endl;
    ptr_target->TakeDamage(dmg);

};

void Pet::TakeDamage(int dmg){
    printf("%d\n", dmg);
    printf("%d\n", cur_hp);
    cur_hp-=dmg;
    cout << name << " at " << cur_hp << " health left!" << endl;

    if (cur_hp <= 0){
        printf("OH NO I\"M DEAD\n");
    }
}

void print_str_array(string msg_list[], int len){

    cout << len << endl;
    for (int i = 0; i < len; i++){
    cout << msg_list[i] << endl;
    }
};

int ask_for_int(string msg, int default_int=0){

    cout << msg << endl;
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

    cout << msg << endl;
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

    //attack enemy
    cout << &enemy_pet << endl;
    p1_pet.Attack(&enemy_pet, 10);

    //do BATTLE one way

    bool battle_done = false;
    while (!battle_done){

        string answer = ask_for_str("Fight?\n");

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
