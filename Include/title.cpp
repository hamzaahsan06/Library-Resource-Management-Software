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

    // ASCII Art for "LIBRARY"
    const std::string LIBRARY_ART =
        "  _         _____    ____   \n"
        " | |       |_   _|  |  _ \\ \n"
        " | |         | |    | |_) | \n"
        " | |         | |    |  _ <| \n"
        " | |____    _| |_   | |_) | \n"
        " |______|  |_____|  | ___ /  \n";

    // ASCII Art for "MANAGEMENT SYSTEM"

    const std::string TITLE = "\n    M A N A G E M E N T";
    const std::string TITLE1 = "\n       S Y S T E M \n\n";
    const std::string QUOTE = "Your mind is your instrument. Learn to be its master and not its slave.";

    std::cout << GOLD << LIBRARY_ART << RESET;
    std::cout << CYAN << TITLE << RESET;
    std::cout << CYAN << TITLE1 << RESET;
    std::cout << ITALIC << WHITE << QUOTE << RESET << std::endl;
}