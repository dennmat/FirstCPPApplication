#include "stdafx.h"
#include "catch.hpp"

#include <vector>
#include <iostream>

#include "utils.h"


TEST_CASE("Utils test", "[utils]") 
{

    SECTION("String joining")
    {
        std::vector<std::string> input;
        REQUIRE("" == StringJoin(input, ','));

        input.push_back("1");
        REQUIRE("1" == StringJoin(input, ','));

        input.push_back("2");
        input.push_back("3");
        REQUIRE("1,2,3" == StringJoin(input, ','));
        REQUIRE("1|2|3" == StringJoin(input, '|'));


    };

    SECTION("String splitting")
    {
        std::vector<std::string> expected;
        expected.push_back("1");
        expected.push_back("2");
        expected.push_back("3");
        REQUIRE(expected == StringSplit("1,2,3", ','));
        REQUIRE(expected == StringSplit("1|2|3", '|'));
        REQUIRE(expected == StringSplit("1||2||3|", '|'));

        expected.clear();
        expected.push_back("This");
        expected.push_back("is");
        expected.push_back("SPARTA!");
        REQUIRE(expected == StringSplit("This is SPARTA!", ' '));

        expected.clear();
        expected.push_back("C:");
        expected.push_back("my");
        expected.push_back("project");
        expected.push_back("dir.exe");
        REQUIRE(expected == StringSplit("C:/my/project/dir.exe", '/'));
    };

    SECTION("Get paths")
    {
        //TODO fix test lower case-ing everything. It's because git bash uses c:
        //and VS2010 uses C: which breaks the paths
        REQUIRE(ToLower(get_exe_path()) == ToLower("c:\\Users\\Mark\\Documents\\Visual Studio 2010\\Projects\\FirstCPPApplication\\Debug\\CATCHCPPTEST.exe"));
        REQUIRE(ToLower(get_data_path()) == ToLower("c:\\Users\\Mark\\Documents\\Visual Studio 2010\\Projects\\FirstCPPApplication\\Debug\\data\\"));
        REQUIRE(ToLower(get_images_path()) == ToLower("c:\\Users\\Mark\\Documents\\Visual Studio 2010\\Projects\\FirstCPPApplication\\Debug\\images\\"));
    };
};
