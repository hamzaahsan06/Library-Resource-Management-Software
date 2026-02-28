// add all classes of users here

#ifndef USER_H
#define USER_H
#include<ctime>
#include <iostream>
#include <string>
#include "Resources.h"
using namespace std;
class User
{
    string password;

protected:
    string username;
    string firstName;
    string lastName;
    string address;
    double balance;
    string borrowedResources[10]; // Assuming a maximum of 2 borrowed resources
    struct BorrowingHistory
    {
        string resource;
        string borrowDate;
        string returnDate;
        string dueDate;
        time_t dueDateRaw; // Store due date as time_t for easier calculations
    } borrowingHistory[10];  // Assuming a maximum of 10 borrowing history records
    int maxBorrowLimit;

public:
    User() : balance(0.0), maxBorrowLimit(2) {}
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
    bool login(string user, string pass)
    {
        return (username == user && password == pass);
    }
    virtual void borrowResource(LibraryResource r) = 0;
    virtual void returnResource(LibraryResource r) = 0;
    virtual void displayInfo() = 0;
    virtual ~User() {}
    string getUsername() const
    {
        if (username.empty())
        {
            cout << "Username cannot be empty." << endl;
            return "";
        }
        return username;
    }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getAddress() const { return address; }
    double getBalance() const { return balance; }
    void setBalance(double b)
    {
        if (b < 0)
        {
            return;
        }
        balance = b;
    }
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
};



class Student: public User{
    int rollNo;
    string department;
    int batch;
    public:
    Student() : User() {
        rollNo = 0;
        department = "Unknown";
        batch = 0;
    } 
    Student(string user, string pass, double bal, string fname, string lname, string addr,
            int roll, string dept, int batch)
        : User(user, pass, bal, fname, lname, addr), rollNo(roll), department(dept), batch(batch) {}
    void borrowResource(LibraryResource r) override{
        if(balance < 0){
            cout<<"Cannot borrow resource. Outstanding balance: "<<balance<<endl;
            return;
        }
        for(int i=0; i<maxBorrowLimit; i++){
            if(borrowedResources[i].empty()){
                if(r.borrowResource()){
                    cout<<"Successfully borrowed "<<r.getTitle()<<endl;
                    borrowedResources[i] = r.getTitle();
                    cout<<"Borrowed: "<<r.getTitle()<<endl;
                    borrowingHistory[i].resource = r.getTitle();
                    time_t now = time(0);
                char buf[80];
                strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
                borrowingHistory[i].borrowDate = buf;

                // --- Set Due Date (e.g., 10 days later) ---
                time_t due = now + (10 * 24 * 60 * 60); // 10 days in seconds
                strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&due));
                borrowingHistory[i].dueDate = buf;

                // --- Set Return Date initially ---
                borrowingHistory[i].returnDate = "Not returned yet";
                } else {
                    cout<<"Failed to borrow "<<r.getTitle()<<". No copies available."<<endl;
                }
                return;
            }
        }
        cout<<"Cannot borrow more resources. Maximum limit reached."<<endl;
    }
    void returnResource(LibraryResource r) override {
    for (int i = 0; i < maxBorrowLimit; i++) {
        if (borrowedResources[i] == r.getTitle()) {
            // Mark resource as returned in the library system
            r.returnResource();
            cout << "Successfully returned " << r.getTitle() << endl;

            // Clear from borrowed list
            borrowedResources[i].clear();

            // --- Set Return Date ---
            time_t now = time(0);
            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
            borrowingHistory[i].returnDate = "Returned on: " + string(buf);

            // --- Fine Calculation ---
            // Instead of parsing dueDate string, store dueDate as time_t in BorrowingHistory
            // Let's assume you add: time_t dueDateRaw; in BorrowingHistory struct
            time_t dueDate = borrowingHistory[i].dueDateRaw;

            if (difftime(now, dueDate) > 0) {
                // Overdue: calculate fine
                int daysLate = static_cast<int>(difftime(now, dueDate) / (60 * 60 * 24));
                double finePerDay = 10.0; // Example fine rate
                double fine = daysLate * finePerDay;

                balance -= fine;
                cout << "Resource was overdue by " << daysLate
                     << " days. Fine of " << fine
                     << " deducted. New balance: " << balance << endl;
            }

            return;
        }
    }
    cout << "You have not borrowed " << r.getTitle() << endl;
}
void displayInfo() override {
    cout << "----- Student Information -----" << endl;
    cout << "Username: " << getUsername() << endl;
    cout << "Name: " << getFirstName() << " " << getLastName() << endl;
    cout << "Address: " << getAddress() << endl;
    cout << "Balance: " << getBalance() << endl;
    cout << "Roll No: " << rollNo << endl;
    cout << "Department: " << department << endl;
    cout << "Batch: " << batch << endl;

    cout << "\nBorrowed Resources:" << endl;
    for (int i = 0; i < maxBorrowLimit; i++) {
        if (!borrowedResources[i].empty()) {
            cout << " - " << borrowedResources[i] << endl;
        }
    }

    cout << "\nBorrowing History:" << endl;
    for (int i = 0; i < 10; i++) {
        if (!borrowingHistory[i].resource.empty()) {
            cout << "Resource: " << borrowingHistory[i].resource << endl;
            cout << "Borrow Date: " << borrowingHistory[i].borrowDate << endl;
            cout << "Due Date: " << borrowingHistory[i].dueDate << endl;
            cout << "Return Date: " << borrowingHistory[i].returnDate << endl;
            cout << "-----------------------------" << endl;
        }
    }
}
};
class Teacher:public User{
    int employeeID;
    string department;
    string designation;
    public:
    Teacher() : User() {
        maxBorrowLimit=5;
        employeeID = 0;
        department = "Unknown";
        designation = "Unknown";
    }
    Teacher(string user, string pass, double bal, string fname, string lname, string addr,
            int empID, string dept, string desig)
        : User(user, pass, bal, fname, lname, addr), employeeID(empID), department(dept), designation(desig) {maxBorrowLimit=5;} 
    void borrowResource(LibraryResource r) override{
        if(balance < 0){
            cout<<"Cannot borrow resource. Outstanding balance: "<<balance<<endl;
            return;
        }
        for(int i=0; i<maxBorrowLimit; i++){
            if(borrowedResources[i].empty()){
                if(r.borrowResource()){
                    cout<<"Successfully borrowed "<<r.getTitle()<<endl;
                    borrowedResources[i] = r.getTitle();
                    cout<<"Borrowed: "<<r.getTitle()<<endl;
                    borrowingHistory[i].resource = r.getTitle();
                    time_t now = time(0);
                char buf[80];
                strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
                borrowingHistory[i].borrowDate = buf;

                // --- Set Due Date (e.g., 14 days later) ---
                time_t due = now + (14 * 24 * 60 * 60); // 14 days in seconds
                strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&due));
                borrowingHistory[i].dueDate = buf;

                // --- Set Return Date initially ---
                borrowingHistory[i].returnDate = "Not returned yet";
                } else {
                    cout<<"Failed to borrow "<<r.getTitle()<<". No copies available."<<endl;
                }
                return;
            }
        }
        cout<<"Cannot borrow more resources. Maximum limit reached."<<endl;
    }
    void returnResource(LibraryResource r) override {
    for (int i = 0; i < maxBorrowLimit; i++) {
        if (borrowedResources[i] == r.getTitle()) {
            // Mark resource as returned in the library system
            r.returnResource();
            cout << "Successfully returned " << r.getTitle() << endl;

            // Clear from borrowed list
            borrowedResources[i].clear();

            // --- Set Return Date ---
            time_t now = time(0);
            char buf[80];
            strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
            borrowingHistory[i].returnDate = "Returned on: " + string(buf);

            // --- Fine Calculation ---
            // Instead of parsing dueDate string, store dueDate as time_t in BorrowingHistory
            // Let's assume you add: time_t dueDateRaw; in BorrowingHistory struct
            time_t dueDate = borrowingHistory[i].dueDateRaw;

            if (difftime(now, dueDate) > 0) {
                // Overdue: calculate fine
                int daysLate = static_cast<int>(difftime(now, dueDate) / (60 * 60 * 24));
                double finePerDay = 5.0; // Example fine rate
                double fine = daysLate * finePerDay;

                balance -= fine;
                cout << "Resource was overdue by " << daysLate
                     << " days. Fine of " << fine
                     << " deducted. New balance: " << balance << endl;
            }

            return;
        }
    }
    cout << "You have not borrowed " << r.getTitle() << endl;
}
    void displayInfo() override {
    cout << "----- Teacher Information -----" << endl;
    cout << "Username: " << getUsername() << endl;
    cout << "Name: " << getFirstName() << " " << getLastName() << endl;
    cout << "Address: " << getAddress() << endl;
    cout << "Balance: " << getBalance() << endl;
    cout << "Employee ID: " << employeeID << endl;
    cout << "Department: " << department << endl;
    cout << "Designation: " << designation << endl;
    }
    void setEmployeeID(int id) { employeeID = id; }
    void setDepartment(string dept) { department = dept; }
    void setDesignation(string desig) { designation = desig; }
    int getemployeeID() const { return employeeID; }
    string getDepartment() const { return department; }
    string getDesignation() const { return designation; }
};
class Admin:public User{
    int adminID;
    public:
    Admin() : User() {
        adminID = 0;
    }
    Admin(string user, string pass, double bal, string fname, string lname, string addr,
            int id)
        : User(user, pass, bal, fname, lname, addr), adminID(id) {}
    void printUserInfo(User* user) {
        cout << "----- User Information -----" << endl;
        cout << "Username: " << user->getUsername() << endl;
        cout << "Name: " << user->getFirstName() << " " << user->getLastName() << endl;
        cout << "Address: " << user->getAddress() << endl;
        cout << "Balance: " << user->getBalance() << endl;
    }
};

// ....

#endif // USER_H