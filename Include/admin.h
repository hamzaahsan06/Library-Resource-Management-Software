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

    // ---------- User Management ----------
    void printAllCustomersReport(Library &lib); // report of all users and borrow history
    void searchUser(Library &lib);              // search user by ID
    void fineManagement(Library &lib);          // view all user balances and fines
    void deleteUser(Library &lib);     // marks user as deleted — does not remove from vector

    // ---------- Resource Management ----------
    void addResource(Library &lib);    // add new resource to library
    void deleteResource(Library &lib); // marks resource as deleted — does not remove from vector
    void updateResource(Library &lib); // update resource details

    // ---------- Circulation / Borrowing Management ----------
    void printIssuedResources(Library &lib);  // currently issued resources
    void printOverdueResources(Library &lib); // overdue resources

    // ---------- Reports & Analytics ----------
    void generateStats(Library &lib);                         // library statistics
    void exportReports(Library &lib, const string &filename); // export report to file

    // ---------- Special / Additional Features ----------
    void collectDonationFromUser(User *u, Library &lib); // adds donated resource and rewards user balance

    // ---------- Display ----------
    void displayInfo() const override;
};

#endif