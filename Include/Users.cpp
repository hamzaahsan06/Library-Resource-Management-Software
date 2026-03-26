#include "Users.h"
#include <iostream>
using namespace std;

// ===================== User =====================

// ---------- Constructors ----------
User::User()
{
    userID = 0;
    type = "unknown";
    username = "Unknown";
    password = "123";
    name = "Unknown";
    address = "Unknown";
    balance = 0.0;
    isDeleted = false;
}

User::User(int ID, string type, string username, string password,
           string name, string address, double balance)
{
    userID = ID;
    this->type = type;
    this->username = username;
    this->password = password;
    this->name = name;
    this->address = address;
    this->balance = balance;
    isDeleted = false;
}

// ---------- Getters ----------
int User::getUserID() const { return userID; }
string User::getType() const { return type; }
string User::getUsername() const { return username; }
string User::getName() const { return name; }
double User::getBalance() const { return balance; }
string User::getAddress() const { return address; }
string User::getPassword() const { return password; }

// ---------- Setters ----------
void User::setAddress(const string &addr) { address = addr; }
void User::setPassword(const string &pass) { password = pass; }
void User::updateBalance(double amount) { balance += amount; } // positive or negative

// ---------- Authentication ----------
bool User::login(string user, string pass)
{
    return (username == user && password == pass);
}

// For marking user as deleted
void User::markDeleted() { isDeleted = true; }
bool User::getIsDeleted() const { return isDeleted; }

// ===================== Student =====================

// ---------- Constructors ----------
Student::Student() : User()
{
    type = "student";
    department = "Unknown";
    rollNo = 0;
}

Student::Student(int ID, string username, string password,
                 string name, string address, double balance,
                 string department, int rollNo)
    : User(ID, "student", username, password, name, address, balance)
{
    this->department = department;
    this->rollNo = rollNo;
}

// ---------- Getters ----------
string Student::getDepartment() const { return department; }
int Student::getRollNo() const { return rollNo; }

// ---------- Role Rules ----------
int Student::getDailyLimit() const { return 2; }     // max 2 resources/day
double Student::getFineRate() const { return 10.0; } // 10 per overdue day
int Student::getBorrowDays() const { return 7; }     // must return in 7 days

// ---------- Display ----------
void Student::displayInfo() const
{
    cout << "ID         : " << userID << endl;
    cout << "Name       : " << name << endl;
    cout << "Role       : Student" << endl;
    cout << "Department : " << department << endl;
    cout << "Roll No    : " << rollNo << endl;
    cout << "Balance    : " << balance << endl;
}

// ===================== Teacher =====================

// ---------- Constructors ----------
Teacher::Teacher() : User()
{
    type = "teacher";
    department = "Unknown";
    designation = "Unknown";
}

Teacher::Teacher(int ID, string username, string password,
                 string name, string address, double balance,
                 string department, string designation)
    : User(ID, "teacher", username, password, name, address, balance)
{
    this->department = department;
    this->designation = designation;
}

// ---------- Getters ----------
string Teacher::getDepartment() const { return department; }
string Teacher::getDesignation() const { return designation; }

// ---------- Role Rules ----------
int Teacher::getDailyLimit() const { return 3; }     // max 3 resources/day
double Teacher::getFineRate() const { return 20.0; } // 20 per overdue day
int Teacher::getBorrowDays() const { return 14; }    // must return in 14 days

// ---------- Display ----------
void Teacher::displayInfo() const
{
    cout << "ID          : " << userID << endl;
    cout << "Name        : " << name << endl;
    cout << "Role        : Teacher" << endl;
    cout << "Department  : " << department << endl;
    cout << "Designation : " << designation << endl;
    cout << "Balance     : " << balance << endl;
}

// ===================== Staff =====================

// ---------- Constructors ----------
Staff::Staff() : User()
{
    type = "staff";
    position = "Unknown";
}

Staff::Staff(int ID, string username, string password,
             string name, string address, double balance,
             string position)
    : User(ID, "staff", username, password, name, address, balance)
{
    this->position = position;
}

// ---------- Getters ----------
string Staff::getPosition() const { return position; }

// ---------- Role Rules ----------
int Staff::getDailyLimit() const { return 4; }    // max 4 resources/day
double Staff::getFineRate() const { return 5.0; } // 5 per overdue day
int Staff::getBorrowDays() const { return 14; }   // must return in 14 days

// ---------- Display ----------
void Staff::displayInfo() const
{
    cout << "ID       : " << userID << endl;
    cout << "Name     : " << name << endl;
    cout << "Role     : Staff" << endl;
    cout << "Position : " << position << endl;
    cout << "Balance  : " << balance << endl;
}

// ===================== PremiumMember =====================

// ---------- Constructors ----------
PremiumMember::PremiumMember() : User()
{
    type = "premium";
    membershipLevel = "Gold";
}

PremiumMember::PremiumMember(int ID, string username, string password,
                             string name, string address, double balance,
                             string level)
    : User(ID, "premium", username, password, name, address, balance)
{
    membershipLevel = level;
}

// ---------- Getters ----------
string PremiumMember::getMembershipLevel() const { return membershipLevel; }

// ---------- Role Rules ----------
int PremiumMember::getDailyLimit() const { return 5; }    // max 5 resources/day
double PremiumMember::getFineRate() const { return 3.0; } // 3 per overdue day
int PremiumMember::getBorrowDays() const { return 30; }   // must return in 30 days

// ---------- Display ----------
void PremiumMember::displayInfo() const
{
    cout << "ID               : " << userID << endl;
    cout << "Name             : " << name << endl;
    cout << "Role             : Premium Member" << endl;
    cout << "Membership Level : " << membershipLevel << endl;
    cout << "Balance          : " << balance << endl;
}