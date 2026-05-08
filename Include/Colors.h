#ifndef COLORS_H
#define COLORS_H



// --- RESET ---
#define RESET           "\033[0m"

// --- SYSTEM FEEDBACK (Used in main.cpp) ---
#define COLOR_ERROR           "\033[1;31m"   // Red Bold
#define COLOR_SYSTEM_NOTIFY   "\033[0;34m"   // Blue Normal
#define COLOR_SUCCESS         "\033[1;32m"   // Green Bold

// --- UI ELEMENTS (Used in Menu.cpp) ---
#define COLOR_MAIN_HEADER     "\033[1;34m"   // Blue Bold
#define COLOR_MENU_OPTION     "\033[1;37m"   // Bright White
#define COLOR_INPUT_PROMPT    "\033[0;36m"   // Cyan Normal
#define COLOR_PROCESS         "\033[90m"     // Dark Gray

#endif // COLORS_H