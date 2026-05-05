#include "admin_menu.h"
#include "../include/utils.h"
#include <iostream>

using namespace std;
using namespace Utils;

void admin_menu(Admin* admin, Library& lib) {
    int adminChoice;
    do {
        clearScreen();
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
        cout << "11. Collect donation\n";
        cout << "12. Fine Management\n";
        cout << "0. Logout\n";

        adminChoice = getValidInt("Enter choice: ");

        switch (adminChoice) {
        case 1:
            clearScreen();
            admin->addResource(lib);
            pauseScreen();
            break;
        case 2:
            clearScreen();
            admin->deleteResource(lib);
            pauseScreen();
            break;
        case 3:
            clearScreen();
            admin->updateResource(lib);
            pauseScreen();
            break;
        case 4:
            clearScreen();
            admin->deleteUser(lib);
            pauseScreen();
            break;
        case 5:
            clearScreen();
            admin->searchUser(lib);
            pauseScreen();
            break;
        case 6:
            clearScreen();
            admin->printAllCustomersReport(lib);
            pauseScreen();
            break;
        case 7:
            clearScreen();
            admin->printIssuedResources(lib);
            pauseScreen();
            break;
        case 8:
            clearScreen();
            admin->printOverdueResources(lib);
            pauseScreen();
            break;
        case 9:
            clearScreen();
            admin->generateStats(lib);
            pauseScreen();
            break;
        case 10:
            clearScreen();
            admin->exportReports(lib, "report.txt");
            pauseScreen();
            break;
        case 11: {
            clearScreen();
            int donorID = getValidInt("Enter User ID of the donor: ");
            User* donor = nullptr;
            for (auto u : lib.getUsers()) {
                if (u->getUserID() == donorID && !u->getIsDeleted()) {
                    donor = u;
                    break;
                }
            }
            if (donor != nullptr) {
                admin->collectDonationFromUser(donor, lib);
            } else {
                cout << "Error: User with ID " << donorID << " not found." << endl;
            }
            pauseScreen();
        }
        break;
        case 12:
            clearScreen();
            admin->fineManagement(lib);
            pauseScreen();
            break;
        case 0:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-10." << endl;
            pauseScreen();
            break;
        }
    } while (adminChoice != 0);
}