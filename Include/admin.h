#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <string>
#include "Users.h"
using namespace std;

class Library; // forward declaration — admin interacts with library

// --------------------- Admin Class ---------------------
class Admin : public User
{
public:
    // ---------- Constructors ----------
    Admin();
    Admin(int ID, string username, string password,
          string name, string address, double balance);

    // ---------- Role Rules ----------
    int getDailyLimit() const override;  // admin does not borrow
    double getFineRate() const override; // no fines for admin
    int getBorrowDays() const override;  // admin does not borrow

    // ---------- Admin Operations ----------
    void printAllCustomersReport(Library &lib);               // report of all users and borrow history
    void addResource(Library &lib);                           // add new resource to library
    void removeResource(Library &lib);                        // remove existing resource
    void updateResource(Library &lib);                        // update resource details
    void printIssuedResources(Library &lib);                  // currently issued resources
    void printOverdueResources(Library &lib);                 // overdue resources
    void generateStats(Library &lib);                         // library statistics
    void exportReports(Library &lib, const string &filename); // export report to file
    void fineManagement(Library &lib);                        // view all user balances and fines

    // ---------- Display ----------
    void displayInfo() const override;
};

#endif