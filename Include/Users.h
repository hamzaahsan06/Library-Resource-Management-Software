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
    bool isDeleted;

public:
    User();
    User(int ID, string type, string username, string password,
         string name, string address, double balance);

    int getUserID() const;
    string getType() const;
    string getUsername() const;
    string getName() const;
    double getBalance() const;
    string getAddress() const;
    string getPassword() const;

    void setAddress(const string &addr);
    void setPassword(const string &pass);
    void updateBalance(double amount);

    // ✅ FIXED (const correctness)
    bool login(const string &user, const string &pass);

    virtual int getDailyLimit() const = 0;
    virtual double getFineRate() const = 0;
    virtual int getBorrowDays() const = 0;

    virtual void displayInfo() const = 0;

    void markDeleted();
    bool getIsDeleted() const;

    virtual ~User() {}
};

// --------------------- Student Class ---------------------
class Student : public User
{
    string department;
    int rollNo;

public:
    Student();
    Student(int ID, string username, string password,
            string name, string address, double balance,
            string department, int rollNo);

    string getDepartment() const;
    int getRollNo() const;

    int getDailyLimit() const override;
    double getFineRate() const override;
    int getBorrowDays() const override;

    void displayInfo() const override;
};

// --------------------- Teacher Class ---------------------
class Teacher : public User
{
    string department;
    string designation;

public:
    Teacher();
    Teacher(int ID, string username, string password,
            string name, string address, double balance,
            string department, string designation);

    string getDepartment() const;
    string getDesignation() const;

    int getDailyLimit() const override;
    double getFineRate() const override;
    int getBorrowDays() const override;

    void displayInfo() const override;
};

// --------------------- Staff Class ---------------------
class Staff : public User
{
    string position;

public:
    Staff();
    Staff(int ID, string username, string password,
          string name, string address, double balance,
          string position);

    string getPosition() const;

    int getDailyLimit() const override;
    double getFineRate() const override;
    int getBorrowDays() const override;

    void displayInfo() const override;
};

// --------------------- Premium Member Class ---------------------
class PremiumMember : public User
{
    string membershipLevel;

public:
    PremiumMember();
    PremiumMember(int ID, string username, string password,
                  string name, string address, double balance,
                  string level);

    string getMembershipLevel() const;

    int getDailyLimit() const override;
    double getFineRate() const override;
    int getBorrowDays() const override;

    void displayInfo() const override;
};

#endif
