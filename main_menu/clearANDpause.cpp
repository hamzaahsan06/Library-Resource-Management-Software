#include "clearANDpause.h"
#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    cout << "\nPress Enter to return to menu...";

    cin.get();
}