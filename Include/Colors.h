#ifndef COLORS_H
#define COLORS_H

// =====================================================
// LIBRARY MANAGEMENT SYSTEM — COLOR DEFINITIONS
// =====================================================
// Enable ANSI colors on Windows (MinGW/GCC) by calling:
//   system("color");   inside main()
// Reset code must be used after every colored print.
// =====================================================

// --- RESET ---
#define RESET           "\033[0m"

// --- I. STRUCTURAL & BRANDING ---
#define COLOR_MAIN_HEADER       "\033[1;34m"   // Blue Bold       — Main Menu Header
#define COLOR_ADMIN_HEADER      "\033[1;35m"   // Magenta Bold    — Admin Menu Header
#define COLOR_USER_HEADER       "\033[1;36m"   // Cyan Bold       — User Menu / Profile Header
#define COLOR_SEPARATOR         "\033[90m"     // Dark Gray (Dim) — Decorative Lines & Separators

// --- II. INTERACTION & NAVIGATION ---
#define COLOR_MENU_OPTION       "\033[1;37m"   // Bright White    — Menu Options
#define COLOR_INPUT_PROMPT      "\033[0;36m"   // Cyan Normal     — Input Prompts
#define COLOR_HINT              "\033[2;37m"   // Dark Gray       — Secondary Labels / Small Hints
#define COLOR_PAUSE             "\033[3;90m"   // Dark Gray Italic — Pause Messages

// --- III. TABLE & DATA DISPLAY ---
#define COLOR_TABLE_HEADER      "\033[1;4;34m" // Blue Bold+Underline — Table Column Headers
#define COLOR_TABLE_ROW         "\033[0;37m"   // White Normal    — Table Data Rows
#define COLOR_STATUS_AVAILABLE  "\033[0;32m"   // Green Normal    — Status: Available
#define COLOR_STATUS_PARTIAL    "\033[0;33m"   // Yellow Normal   — Status: Partially Available
#define COLOR_STATUS_BORROWED   "\033[0;31m"   // Red Normal      — Status: Borrowed / Unavailable

// --- IV. ALERTS & FEEDBACK ---
#define COLOR_SUCCESS           "\033[1;32m"   // Green Bold      — Success Messages
#define COLOR_ERROR             "\033[1;31m"   // Red Bold        — Error Messages
#define COLOR_FINE_WARNING      "\033[1;37;41m"// White on Red Bg — Fines & Urgent Warnings
#define COLOR_STATS             "\033[1;33m"   // Yellow Bold     — Statistics / Report Values

// --- V. SYSTEM STATE ---
#define COLOR_PROCESS           "\033[90m"     // Dark Gray       — Process Messages
#define COLOR_SYSTEM_NOTIFY     "\033[0;34m"   // Blue Normal     — System Notifications

#endif // COLORS_H
