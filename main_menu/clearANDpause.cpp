#include<iostream>
using namespace std;
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#endif
}

void pauseScreen()
{
    cout << "\nPress Enter to return to menu...";

    cin.get();
}