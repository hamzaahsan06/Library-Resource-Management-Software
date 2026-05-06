#include "Utils.h"
#include <limits>
#include <cctype>
#include <algorithm>
#include <stdexcept>
using namespace std;

namespace Utils
{
    int getValidInt(const string &prompt)
    {
        int value;
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Expected an integer input.");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }

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

    double getValidDouble(const string &prompt)
    {
        double value;
        cout << prompt;
        cin >> value;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Expected a decimal number.");
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
    }
}
