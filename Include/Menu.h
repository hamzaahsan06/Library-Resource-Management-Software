#ifndef MENU_H
#define MENU_H

#include "Library.h"
#include "Admin.h"

// Entry point for the UI called by main.cpp
void runLibrarySystem(Library& lib);

// Internal sub-menus
void handleAdminMenu(Admin* admin, Library& lib);
void handleUserMenu(User* loggedUser, Library& lib);

#endif