#include "Utils.h"
#include <limits>
#include <cctype>
#include <algorithm>
using namespace std;
namespace Utils
{
    int getValidInt(const string &prompt)
    {
        int value;
        while (true)
        {
            cout << prompt;
            cin >> value;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Invalid input. Please enter an integer.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

    string getValidString(const string &prompt)
    {
        string value;
        while (true)
        {
            cout << prompt;
            getline(cin, value);

            // Check if the string contains only digits
            bool allDigits = !value.empty() &&
                             all_of(value.begin(), value.end(), ::isdigit);

            if (allDigits)
            {
                cerr << "Invalid input. Please enter text, not just numbers.\n";
            }
            else if (value.empty())
            {
                cerr << "Input cannot be empty. Try again.\n";
            }
            else
            {
                return value;
            }
        }
    }
    double getValidDouble(const string &prompt)
    {
        double value;
        while (true)
        {
            cout << prompt;
            cin >> value;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cerr << "Invalid input. Please enter a decimal number.\n";
            }
            else
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }
}
