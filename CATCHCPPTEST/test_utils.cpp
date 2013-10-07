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
        //TODO fix un-ideal test
        REQUIRE(StringSplit(get_exe_path(), kPathSeparator).back() == "CATCHCPPTEST.exe");
        REQUIRE(StringSplit(get_exe_path(), kPathSeparator).back() == "CATCHCPPTEST.exe");
    };
};
