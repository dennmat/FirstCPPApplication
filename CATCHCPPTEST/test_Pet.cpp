#include "stdafx.h"
#include "catch.hpp"

#include "pet.h"


TEST_CASE ( "Testing pet stuff", "[pet]") {

    Pet * test_pet = new Pet;
    Pet * attack_pet = new Pet;
    test_pet->name = "The Test Pet";

    SECTION ( "A new pet doesn't have an owner by default" ) {
        REQUIRE( test_pet->master == NULL);
        REQUIRE( test_pet->name == "The Test Pet");
    }

    SECTION ( "Pet takes damage and attacks" ) {
        int orig_hp = test_pet->cur_hp;
        int dmg = 10;
        test_pet->TakeDamage(attack_pet, dmg);
        REQUIRE( test_pet->cur_hp == orig_hp - dmg);

        test_pet->Attack(attack_pet, dmg);
        REQUIRE( attack_pet->cur_hp == orig_hp - dmg);
        test_pet->Attack(attack_pet, attack_pet->cur_hp);
        REQUIRE( attack_pet->CheckDeath() == true);
    }

    SECTION ( "Pet remembers last attacker" ) {
        bool pet_remembers = false;
        if(std::find(test_pet->attackers->begin(), test_pet->attackers->end(), attack_pet) != test_pet->attackers->end()) 
        {
            pet_remembers = true;
        }
        else 
        {
            pet_remembers = false;
        }
        REQUIRE( pet_remembers == false);

        attack_pet->Attack(test_pet, 10);
        if(std::find(test_pet->attackers->begin(), test_pet->attackers->end(), attack_pet) != test_pet->attackers->end()) 
        {
            pet_remembers = true;
        }
        else 
        {
            pet_remembers = false;
        }
        REQUIRE( pet_remembers == true);
    }


};
