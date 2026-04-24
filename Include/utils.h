#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
using namespace std;
namespace Utils
{
    int getValidInt(const string &prompt);
    double getValidDouble(const string &prompt);
    string getValidString(const string &prompt);
}

#endif
