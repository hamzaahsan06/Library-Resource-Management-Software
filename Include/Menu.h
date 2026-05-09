#ifndef MENU_H
#define MENU_H

#include "Library.h"
#include "Admin.h"

// main menu loop for the library system
void runLibrarySystem(Library &lib);

// separate menus for admin and regular users
void handleAdminMenu(Admin *admin, Library &lib);
void handleUserMenu(User *loggedUser, Library &lib);

#endif