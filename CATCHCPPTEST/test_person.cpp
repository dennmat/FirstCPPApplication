#include "stdafx.h"
#include "catch.hpp"

#include <actors\Person.h>

TEST_CASE( "actors are tested", "[idk actors]" ) {
    Person* pers = new Person;
    pers->name = "ASD";
    REQUIRE( pers->name == "ASD" );
    REQUIRE( pers->name != "TEST" );
};
