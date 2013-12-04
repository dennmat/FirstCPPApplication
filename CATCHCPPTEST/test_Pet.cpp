#include "stdafx.h"
#include "catch.hpp"

#include "combat.h"


//TEST_CASE ( "Testing Combat stuff", "[Combat]") {
//
//    Combat * test_Combat = new Combat;
//    Combat * attack_Combat = new Combat;
//    //test_Combat->name = "The Test Combat";
//
//    SECTION ( "A new Combat doesn't have an owner by default" ) {
//        REQUIRE( test_Combat->master == NULL);
//        //REQUIRE( test_Combat->name == "The Test Combat");
//    }
//
//    SECTION ( "Combat takes damage and attacks" ) {
//        int orig_hp = test_Combat->cur_hp;
//        int dmg = 10;
//        test_Combat->TakeDamage(attack_Combat, dmg);
//        REQUIRE( test_Combat->cur_hp == orig_hp - dmg);
//
//        test_Combat->Attack(attack_Combat, dmg);
//        REQUIRE( attack_Combat->cur_hp == orig_hp - dmg);
//        test_Combat->Attack(attack_Combat, attack_Combat->cur_hp);
//        REQUIRE( attack_Combat->CheckDeath() == true);
//    }
//
//    SECTION ( "Combat remembers last attacker" ) {
//        bool Combat_remembers = false;
//        if(std::find(test_Combat->attackers->begin(), test_Combat->attackers->end(), attack_Combat) != test_Combat->attackers->end()) 
//        {
//            Combat_remembers = true;
//        }
//        else 
//        {
//            Combat_remembers = false;
//        }
//        REQUIRE( Combat_remembers == false);
//
//        attack_Combat->Attack(test_Combat, 10);
//        if(std::find(test_Combat->attackers->begin(), test_Combat->attackers->end(), attack_Combat) != test_Combat->attackers->end()) 
//        {
//            Combat_remembers = true;
//        }
//        else 
//        {
//            Combat_remembers = false;
//        }
//        REQUIRE( Combat_remembers == true);
//    }
//
//
//};
