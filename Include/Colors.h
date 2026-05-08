#ifndef COLORS_H
#define COLORS_H

// RESET
#define RESET           "\033[0m"    // Resets all formatting to terminal default

// BASIC & BOLD TEXT
#define WHITE           "\033[1;37m" // Bold/Bright White
#define BOLD_WHITE      "\033[1;37m" // Bold/Bright White
#define BOLD_RED        "\033[1;31m" // Bold/Bright Red
#define BOLD_YELLOW     "\033[1;33m" // Bold/Bright Yellow
#define DARK_GRAY       "\033[90m"   // Dark Gray (Bright Black)

// MAIN UI
#define COLOR_MAIN_HEADER     "\033[1;34m" // Bold Blue
#define COLOR_SECTION         "\033[1;36m" // Bold Cyan
#define COLOR_TABLE_HEADER    "\033[1;33m" // Bold Yellow
#define COLOR_SEPARATOR       "\033[90m"   // Dark Gray
#define COLOR_SYSTEM_NOTIFY   "\033[1;36m" // Bold Cyan

// TEXT TYPES
#define COLOR_TITLE           "\033[1;37m" // Bold White
#define COLOR_TYPE            "\033[0;35m" // Standard Magenta
#define COLOR_ID              "\033[1;33m" // Bold Yellow
#define COLOR_DATE            "\033[0;36m" // Standard Cyan

// STATUS
#define COLOR_SUCCESS         "\033[1;32m" // Bold Green
#define COLOR_ERROR           "\033[1;31m" // Bold Red
#define COLOR_WARNING         "\033[1;33m" // Bold Yellow
#define COLOR_ACTIVE          "\033[1;32m" // Bold Green
#define COLOR_DELETED         "\033[1;31m" // Bold Red

// RESOURCE STATUS
#define COLOR_AVAILABLE       "\033[1;32m" // Bold Green
#define COLOR_UNAVAILABLE     "\033[1;31m" // Bold Red

// FINANCIAL
#define COLOR_BALANCE         "\033[1;35m" // Bold Magenta
#define COLOR_FINE            "\033[1;31m" // Bold Red

// INPUT / MENU
#define COLOR_INPUT_PROMPT    "\033[0;36m" // Standard Cyan
#define COLOR_MENU_OPTION     "\033[1;37m" // Bold White

// PROCESS
#define COLOR_PROCESS         "\033[90m"   // Dark Gray

#endif