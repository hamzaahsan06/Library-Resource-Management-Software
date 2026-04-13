#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <string>
using namespace std;

// --------------------- Base User Class ---------------------
class User
{
protected:
    int userID;
    string type; // "student", "teacher", "staff", "premium"
    string username;
    string password;
    string name;
    string address;
    double balance;
    bool isDeleted; // marks user as deleted without removing from vector

public:
    // ---------- Constructors ----------
    User();
    User(int ID, string type, string username, string password,
         string name, string address, double balance);

    // ---------- Getters ----------
    int getUserID() const;
    string getType() const;
    string getUsername() const;
    string getName() const;
    double getBalance() const;
    string getAddress() const;
    string getPassword() const;

    // ---------- Setters ----------
    void setAddress(const string &addr);
    void setPassword(const string &pass);
    void updateBalance(double amount); // positive to add, negative to deduct

    // ---------- Authentication ----------
    bool login(string user, string pass);

    // ---------- Role-based Rules ----------
    virtual int getDailyLimit() const = 0;  // max resources per day — differs by type
    virtual double getFineRate() const = 0; // fine per overdue day — differs by type
    virtual int getBorrowDays() const = 0;  // max days to keep resource — differs by type

    // ---------- Display ----------
    virtual void displayInfo() const = 0;

    // For marking user as deleted
    void markDeleted();        // marks user as deleted
    bool getIsDeleted() const; // returns deleted status

    virtual ~User() {}
};

// --------------------- Student Class ---------------------
class Student : public User
{
    string department;
    int rollNo;

public:
    // ---------- Constructors ----------
    Student();
    Student(int ID, string username, string password,
            string name, string address, double balance,
            string department, int rollNo);

    // ---------- Getters ----------
    string getDepartment() const;
    int getRollNo() const;

    // ---------- Role Rules ----------
    int getDailyLimit() const override;  // max 2 resources/day
    double getFineRate() const override; // 10 per overdue day
    int getBorrowDays() const override;  // 7 days to return

    // ---------- Display ----------
    void displayInfo() const override;
};

// --------------------- Teacher Class ---------------------
class Teacher : public User
{
    string department;
    string designation;

public:
    // ---------- Constructors ----------
    Teacher();
    Teacher(int ID, string username, string password,
            string name, string address, double balance,
            string department, string designation);

    // ---------- Getters ----------
    string getDepartment() const;
    string getDesignation() const;

    // ---------- Role Rules ----------
    int getDailyLimit() const override;  // max 3 resources/day
    double getFineRate() const override; // 20 per overdue day
    int getBorrowDays() const override;  // 14 days to return

    // ---------- Display ----------
    void displayInfo() const override;
};

// --------------------- Staff Class ---------------------
class Staff : public User
{
    string position;

public:
    // ---------- Constructors ----------
    Staff();
    Staff(int ID, string username, string password,
          string name, string address, double balance,
          string position);

    // ---------- Getters ----------
    string getPosition() const;

    // ---------- Role Rules ----------
    int getDailyLimit() const override;  // max 4 resources/day
    double getFineRate() const override; // 5 per overdue day
    int getBorrowDays() const override;  // 14 days to return

    // ---------- Display ----------
    void displayInfo() const override;
};

// --------------------- Premium Member Class ---------------------
class PremiumMember : public User
{
    string membershipLevel;

public:
    // ---------- Constructors ----------
    PremiumMember();
    PremiumMember(int ID, string username, string password,
                  string name, string address, double balance,
                  string level);

    // ---------- Getters ----------
    string getMembershipLevel() const;

    // ---------- Role Rules ----------
    int getDailyLimit() const override;  // max 5 resources/day
    double getFineRate() const override; // 3 per overdue day
    int getBorrowDays() const override;  // 30 days to return

    // ---------- Display ----------
    void displayInfo() const override;
};

#endif
