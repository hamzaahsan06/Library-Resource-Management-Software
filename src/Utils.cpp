#include "../include/Utils.h"
#include <limits>
#include <cctype>
#include <algorithm>
#include <stdexcept>

using namespace std;

namespace Utils
{
    // input validation functions
    int getValidInt(const string &prompt)
    {
        int value;
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            // Clear the error state and ignore the rest of the line to prevent infinite loop
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Expected an integer input.");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
    // validates that input is not empty and not just digits
    string getValidString(const string &prompt)
    {
        string value;
        cout << prompt;
        getline(cin, value);

        bool allDigits = !value.empty() &&
                         all_of(value.begin(), value.end(), ::isdigit);

        if (allDigits)
        {
            throw invalid_argument("Expected text input, not just digits.");
        }
        else if (value.empty())
        {
            throw invalid_argument("Input cannot be empty.");
        }

        return value;
    }
    // validates that input is a valid double (for balance input)
    double getValidDouble(const string &prompt)
    {
        double value;
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            // Clear the error state and ignore the rest of the line to prevent infinite loop
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Expected a decimal number.");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }

    // Clears the terminal screen based on the operating system
    void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // Pauses execution and waits for user input to continue
    void pauseScreen()
    {
        cout << "\nPress Enter to return to menu...";
        cin.get();
    }
}
