#include <iostream>
#include "title.h"
using namespace std;

// ANSI Color Codes
const std::string RESET = "\033[0m";
const std::string GOLD = "\033[38;5;220m"; // Vibrant Gold/Yellow
const std::string CYAN = "\033[36m";       // Cyan
const std::string WHITE = "\033[37m";      // White
const std::string ITALIC = "\033[3m";      // Italic text

void showTitle()
{
    system("cls"); // for Windows

    // ASCII Art for "L M S"
    const std::string LMS_ART =
        "  _          __  __     _____ \n"
        " | |        |  \\/  |   / ____|\n"
        " | |        | \\  / |  | (___  \n"
        " | |        | |\\/| |   \\___ \\ \n"
        " | |____    | |  | |   ____) |\n"
        " |______|   |_|  |_|  |_____/ \n";

    // Updated with RESOURCE[cite: 1]
    const std::string SUB_TITLE1 = "\n\n  L I B R A R Y   &   R E S O U R C E";
    const std::string SUB_TITLE2 = "\n        M A N A G E M E N T";
    const std::string SUB_TITLE3 = "\n            S Y S T E M \n\n";

    const std::string QUOTE_TEXT = "\"Your mind is your instrument. Learn to be its master and not its slave.\"";

    // Displaying the Titles
    std::cout << GOLD << LMS_ART << RESET;
    std::cout << CYAN << SUB_TITLE1 << RESET;
    std::cout << CYAN << SUB_TITLE2 << RESET;
    std::cout << CYAN << SUB_TITLE3 << RESET;

    // Small single-line border for the quote
    std::cout << CYAN << " -----------------------------------------------------------------------------" << std::endl;
    std::cout << " | " << GOLD << ITALIC << QUOTE_TEXT << RESET << CYAN << " |" << std::endl;
    std::cout << " -----------------------------------------------------------------------------" << RESET << std::endl;

    cout << "\nPress Enter to explore....";
    cin.get();
}