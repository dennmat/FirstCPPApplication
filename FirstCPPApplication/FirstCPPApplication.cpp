#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include <cstdlib>
#include <algorithm>
#include <windows.h>

#include "libtcod_cpp_hpp\libtcod.hpp"

#include "FirstCPPApplication.h"

#include "utils.h"
#include <actors\Person.h>
#include "combat.h"
#include "map.h"
#include "game.h"
#include "input.h"

using namespace std;


void WelcomeMessage(){
    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    printf("\n\t\t   Prepare to die!\n\n");

    printf("Most importantly, ignore the instructions that aren't the right ones. \n");
    printf("NSWE or num pad to move\n");
    printf("Move into someone to attack\n");
    printf("Move into a door then press o to open or close it. It's dependant on the direction you're facing\n");

    printf("\n\nPress i to open inventory\n");
    printf("\nIn the inventory:");
    printf("\nAlphabetically, a is your first item, b is your second, z is your 26th or whatever\n");
    printf("You need to choose and then activate the item by pressing the appropriate button\n and then the button again to activate it\n");
    printf("Then you can press e to use the item\n");
    printf("If you eat corpses you get one health per turn\n");
    printf("You should have an item that you can eat in your inventory right now that gives 1k hp\n");
    printf("\n");
    printf("You'll get there, big guy\n");
};

int* getConsoleSize(){

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // printf("columns: %d\n", columns);
    // printf("rows: %d\n", rows);

    int *info = new int[2];
    info[0] = columns;
    info[1] = rows;
    return info;
};

void clearScreen()
{
    system("cls");
}
