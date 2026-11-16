#include "Screen.h"

#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

void clearScreen() {
    #ifdef _WIN32
    system("cls");

    #else
    system("clear");

    #endif
}

void pauseScreen() {
    cout <<"\033[32m" << "\nPress Enter to continue .....";
    string pause;
    getline(cin, pause);
}