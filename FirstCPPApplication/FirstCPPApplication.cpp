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
#include "Person.h"
#include "combat.h"
#include "map.h"
#include "game.h"
#include "input.h"

using namespace std;


void WelcomeMessage(){
    printf("\t\tWELCOME TO THE GAME!\n");
    printf("\t\t********************\n");
    printf("\n\t\t   Prepare to die!\n\n");
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
