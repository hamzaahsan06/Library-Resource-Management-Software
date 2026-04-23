#include <iostream>
#include "include/Library.h"
#include "include/admin.h"
#include "FileHandling/FileHandler.h"
using namespace std;

int main()
{
    Library lib("My Library");

    // ---------- Load Data from CSV ----------
    ReadUsersFromFile("database/users.csv", lib.getUsers());
    ReadResourcesFromFile("database/resources.csv", lib.getResources());

    int choice;

    do
    {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. View Available Resources (Guest)\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        try
        {
            if (choice == 1)
            {
                choice=0;
                User *loggedUser = lib.loginUser();

                if (loggedUser == nullptr)
                {
                    continue;
                }

                // ---------- ADMIN ----------
                if (loggedUser->getType() == "admin")
                {
                    Admin *admin = dynamic_cast<Admin *>(loggedUser);
                    int adminChoice;

                    
                        try
                        {
                            do
                    {
                            
                        cout << "\n===== ADMIN MENU =====\n";
                        cout << "1. Add Resource\n";
                        cout << "2. Delete Resource\n";
                        cout << "3. Update Resource\n";
                        cout << "4. Delete User\n";
                        cout << "5. Search User\n";
                        cout << "6. View All Customers Report\n";
                        cout << "7. Issued Resources\n";
                        cout << "8. Overdue Resources\n";
                        cout << "9. Statistics\n";
                        cout << "10. Export Report\n";
                        cout << "0. Logout\n";
                        cout << "Enter choice: ";
                        cin >> adminChoice;

                        if(cin.fail()){
                            throw runtime_error("Invalid input: expected an integer!");
                        }
                        cout<<"You entered: "<< adminChoice<<endl;

                        switch (adminChoice)
                        {
                        case 1:
                            admin->addResource(lib);
                            break;
                        case 2:
                            admin->deleteResource(lib);
                            break;
                        case 3:
                            admin->updateResource(lib);
                            break;
                        case 4:
                            admin->deleteUser(lib);
                            break;
                        case 5:
                            admin->searchUser(lib);
                            break;
                        case 6:
                            admin->printAllCustomersReport(lib);
                            break;
                        case 7:
                            admin->printIssuedResources(lib);
                            break;
                        case 8:
                            admin->printOverdueResources(lib);
                            break;
                        case 9:
                            admin->generateStats(lib);
                            break;
                        case 10:
                            admin->exportReports(lib, "report.txt");
                            break;
                            
                        }
                        } while (adminChoice != 0);

                        }
                        catch(const exception& e)
                        {
                            cout <<"Error: "<< e.what() << '\n';
                        }
                        
                    
                }
                else
                {
                    // ---------- NORMAL USER ----------
                    int userChoice;

                    try
                    {
                        do
                    {
                        cout << "\n===== USER MENU =====\n";
                        cout << "1. View Profile\n";
                        cout << "2. Change Password\n";
                        cout << "3. View Available Resources\n";
                        cout << "4. Search Resources\n";
                        cout << "5. Borrow Resource\n";
                        cout << "6. Return Resource\n";
                        cout << "0. Logout\n";
                        cout << "Enter choice: ";
                        cin >> userChoice;

                    if(cin.fail()){
                        throw runtime_error("Invalid input: expected an integer!");
                    }

                        switch (userChoice)
                        {
                        case 1:
                            lib.showUserProfile(loggedUser);
                            break;

                        case 2:
                            lib.changePassword(loggedUser);
                            break;

                        case 3:
                            lib.showAvailableResources();
                            break;

                        case 4:
                            lib.searchResources();
                            break;

                        case 5:
                        {
                            int id;
                            cout << "Enter Resource ID: ";
                            cin >> id;

                            for (auto r : lib.getResources())
                            {
                                if (r->getResourceID() == id && !r->getIsDeleted())
                                {
                                    lib.borrowResource(loggedUser, r);
                                    break;
                                }
                            }
                            break;
                        }

                        case 6:
                        {
                            int id;
                            cout << "Enter Resource ID: ";
                            cin >> id;

                            for (auto r : lib.getResources())
                            {
                                if (r->getResourceID() == id)
                                {
                                    lib.returnResource(loggedUser, r);
                                    break;
                                }
                            }
                            break;
                        }
                        }

                    } while (userChoice != 0);
                    }
                    catch(const exception& e)
                    {
                        cout << e.what() << '\n';
                    }
                    
                }
            }

            else if (choice == 2)
            {
                try
                {
                    choice=0;
                    lib.registerUser();
                }
                catch(const exception& e)
                {
                    SaveUsersToFile("database/users.csv", lib.getUsers());
    SaveResourcesToFile("database/resources.csv", lib.getResources());
                    cout<< e.what() << '\n';
                }
                
            }

            else if (choice == 3)
            {
                lib.showAvailableResources();
            }

            else if (choice == 0)
            {
                cout << "Exiting system...\n";
            }

            else
            {
                throw runtime_error("Invalid menu choice!");
            }
        }
        catch (exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }

    } while (choice != 0);

    // ---------- Save Data Back to CSV ----------
    SaveUsersToFile("database/users.csv", lib.getUsers());
    SaveResourcesToFile("database/resources.csv", lib.getResources());

    return 0;
}