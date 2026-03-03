

// add all classes of users here

#ifndef USER_H
#define USER_H
#include <ctime>
#include <iostream>
#include <string>
#include "Resources.h"
using namespace std;


class User
{
    string password;

protected:
    int UserID;
    string username;
    string firstName;
    string lastName;
    string address;
    double balance;
    static int idCounter; // shared counter to auto-generate unique IDs

public:
    // constructors
    User()
    {
        UserID = ++idCounter;
        username = "xyz";
        firstName = "X";
        lastName = "Y";
        address = "AAA";
        balance = 0.0;
    }
    User(string user, string pass, double bal, string fname, string lname, string addr)
    {
        if (bal < 0)
        {
            cout << "Balance cannot be negative. Account creation failed." << endl;
            return;
        }
        if (user.empty() || pass.empty() || fname.empty() || lname.empty() || addr.empty())
        {
            cout << "All fields are required. Account creation failed." << endl;
            return;
        }
        username = user;
        password = pass;
        firstName = fname;
        lastName = lname;
        address = addr;
    };

    // Getters
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getAddress() const { return address; }
    double getBalance() const { return balance; }

    string getUsername() const
    {
        if (username.empty())
        {
            cout << "Username cannot be empty." << endl;
            return "";
        }
        return username;
    }

    // Setters
    void setAddress(string addr) { address = addr; }
    void setFirstName(string fname) { firstName = fname; }
    void setLastName(string lname) { lastName = lname; }
    void setPassword(string pass)
    {
        if (!pass.empty())
        {
            return;
        }
        password = pass;
    }
    void setBalance(double b)
    {
        if (b < 0)
        {
            return;
        }
        balance = b;
    }

    // methods
    bool login(string user, string pass)
    {
        return (username == user && password == pass);
    }
    virtual void displayInfo() = 0;
    virtual bool can_borrow() = 0; 
    virtual ~User() {}
};

class Student : public User
{
    int rollNo;
    string department;
    int batch;

public:
    // Consturctors
    Student() : User()
    {
        rollNo = 0;
        department = "Unknown";
        batch = 0;
    }
    Student(string user, string pass, double bal, string fname, string lname, string addr,
            int roll, string dept, int batch)
        : User(user, pass, bal, fname, lname, addr), rollNo(roll), department(dept), batch(batch) {}

    // Methods
    void displayInfo() override
    {
        cout << "----- Student Information -----" << endl;
        cout << "Username: " << getUsername() << endl;
        cout << "Name: " << getFirstName() << " " << getLastName() << endl;
        cout << "Address: " << getAddress() << endl;
        cout << "Balance: " << getBalance() << endl;
        cout << "Roll No: " << rollNo << endl;
        cout << "Department: " << department << endl;
        cout << "Batch: " << batch << endl;
    }

    // getters
    int getRollNo() const { return rollNo; }
    string getDepartment() const { return department; }
    int getBatch() const { return batch; }

    // setters
    void setRollNo(int r) { rollNo = r; }
    void setDepartment(string d) { department = d; }
    void setBatch(int b) { batch = b; }
};
class Teacher : public User
{
    string department;
    string designation;

public:
    // Consturctors
    Teacher() : User()
    {
        department = "Unknown";
        designation = "Unknown";
    }
    Teacher(string user, string pass, double bal, string fname, string lname, string addr,
            int empID, string dept, string desig)
        : User(user, pass, bal, fname, lname, addr), department(dept), designation(desig) {}

    // Methods
    void displayInfo() override
    {
        cout << "----- Teacher Information -----" << endl;
        cout << "Username: " << getUsername() << endl;
        cout << "Name: " << getFirstName() << " " << getLastName() << endl;
        cout << "Address: " << getAddress() << endl;
        cout << "Balance: " << getBalance() << endl;
        cout << "Department: " << department << endl;
        cout << "Designation: " << designation << endl;
    }

    // Setters
    void setDepartment(string dept) { department = dept; }
    void setDesignation(string desig) { designation = desig; }

    // Getters
    string getDepartment() const { return department; }
    string getDesignation() const { return designation; }
};
class Staff : public User
{
    string department;
    string position;

public:
    // Consturctors
    Staff(string user, string pass, double bal, string fname, string lname, string addr,
          int id, string dept, string pos) : User(user, pass, bal, fname, lname, addr),
                                             department(dept), position(pos) {}

    // Methods
    void displayInfo() override
    {
        cout << "----- Staff Information -----" << endl;
        cout << "Username: " << getUsername() << endl;
        cout << "Name: " << getFirstName() << " " << getLastName() << endl;
        cout << "Address: " << getAddress() << endl;
        cout << "Balance: " << getBalance() << endl;
        cout << "Department: " << department << endl;
        cout << "Position: " << position << endl;
    }
    // setters
    void setDepartment(string d) { department = d; }
    void setPosition(string p) { position = p; }
    // getters
    string getDepartment() const { return department; }
    string getPosition() const { return position; }
};


#endif // USER_H
