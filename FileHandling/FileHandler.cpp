#include "FileHandler.h"
#include <iostream>
using namespace std;

// CSV column order:
// userID,type,username,password,name,address,balance,department,rollNo,designation,position,membershipLevel

// reads each row, creates correct derived object based on type, pushes into vector
void ReadUsersFromFile(const string &filename, vector<User *> &users)
{
    ifstream readFile(filename);

    if (!readFile.is_open())
    {
        cout << "Error: Could not open " << filename << endl;
        return;
    }

    string line;
    getline(readFile, line); // skip header row

    while (getline(readFile, line))
    {
        stringstream ss(line);

        // read common columns — same for all user types
        string idSTR, type, username, password, name, address, balSTR;

        getline(ss, idSTR, ',');    // column 1
        getline(ss, type, ',');     // column 2
        getline(ss, username, ','); // column 3
        getline(ss, password, ','); // column 4
        getline(ss, name, ',');     // column 5
        getline(ss, address, ',');  // column 6
        getline(ss, balSTR, ',');   // column 7

        int id = stoi(idSTR);
        double bal = stod(balSTR);

        if (type == "student")
        {
            string department, rollSTR;
            getline(ss, department, ','); // column 8
            getline(ss, rollSTR, ',');    // column 9

            User *u = new Student(id, username, password, name, address, bal, department, stoi(rollSTR));
            users.push_back(u);
        }
        else if (type == "teacher")
        {
            string skip, department, designation;
            getline(ss, department, ',');  // column 8
            getline(ss, skip, ',');        // column 9 — blank (rollNo)
            getline(ss, designation, ','); // column 10

            User *u = new Teacher(id, username, password, name, address, bal, department, designation);
            users.push_back(u);
        }
        else if (type == "staff")
        {
            string skip, position;
            getline(ss, skip, ',');     // column 8  — blank (department)
            getline(ss, skip, ',');     // column 9  — blank (rollNo)
            getline(ss, skip, ',');     // column 10 — blank (designation)
            getline(ss, position, ','); // column 11

            User *u = new Staff(id, username, password, name, address, bal, position);
            users.push_back(u);
        }
        else if (type == "premium")
        {
            string skip, membershipLevel;
            getline(ss, skip, ',');            // column 8  — blank (department)
            getline(ss, skip, ',');            // column 9  — blank (rollNo)
            getline(ss, skip, ',');            // column 10 — blank (designation)
            getline(ss, skip, ',');            // column 11 — blank (position)
            getline(ss, membershipLevel, ','); // column 12

            User *u = new PremiumMember(id, username, password, name, address, bal, membershipLevel);
            users.push_back(u);
        }
        else if (type == "admin")
        {
            // no additional attributes
            User *u = new Admin(id, username, password, name, address, bal);
            users.push_back(u);
        }
    }

    readFile.close();
}

// rewrites entire CSV with current vector data — call after any modification
void SaveUsersToFile(const string &filename, vector<User *> &users)
{
    ofstream writeFile(filename);

    if (!writeFile.is_open())
    {
        cout << "Error: Could not open " << filename << endl;
        return;
    }

    writeFile << "userID,type,username,password,name,address,balance,"
              << "department,rollNo,designation,position,membershipLevel\n";

    for (auto user : users)
    {
        // write common columns
        writeFile << user->getUserID() << ","
                  << user->getType() << ","
                  << user->getUsername() << ","
                  << user->getPassword() << ","
                  << user->getName() << ","
                  << user->getAddress() << ","
                  << user->getBalance() << ",";

        if (user->getType() == "student")
        {
            Student *s = dynamic_cast<Student *>(user);
            writeFile << s->getDepartment() << "," // column 8
                      << s->getRollNo() << ","     // column 9
                      << ",," << "\n";             // columns 10,11,12 blank
        }
        else if (user->getType() == "teacher")
        {
            Teacher *t = dynamic_cast<Teacher *>(user);
            writeFile << t->getDepartment() << ","  // column 8
                      << ","                        // column 9 blank
                      << t->getDesignation() << "," // column 10
                      << "," << "\n";               // columns 11,12 blank
        }
        else if (user->getType() == "staff")
        {
            Staff *s = dynamic_cast<Staff *>(user);
            writeFile << ",,,"                   // columns 8,9,10 blank
                      << s->getPosition() << "," // column 11
                      << "\n";                   // column 12 blank
        }
        else if (user->getType() == "premium")
        {
            PremiumMember *p = dynamic_cast<PremiumMember *>(user);
            writeFile << ",,,,"                           // columns 8,9,10,11 blank
                      << p->getMembershipLevel() << "\n"; // column 12
        }
        else if (user->getType() == "admin")
        {
            writeFile << ",,,,\n"; // columns 8-12 all blank
        }
    }

    writeFile.close();
}

// CSV column order:
// resourceID,type,title,authorCreator,category,totalCopies,availableCopies,availabilityStatus,
// ISBN,publisher,yearPublished,director,durationMinutes,genre,narrator,format,volumeNumber,issueNumber,publicationDate,editionDate,region

void ReadResourcesFromFile(const string &filename, vector<LibraryResource *> &resources)
{
    ifstream readFile(filename);

    if (!readFile.is_open())
    {
        cout << "Error: Could not open " << filename << endl;
        return;
    }

    string line;
    getline(readFile, line); // skip header row

    while (getline(readFile, line))
    {
        stringstream ss(line);

        // read common columns — same for all resource types
        string idSTR, type, title, authorCreator, category, totalSTR, availableSTR, availabilityStatus;

        getline(ss, idSTR, ',');              // column 1
        getline(ss, type, ',');               // column 2
        getline(ss, title, ',');              // column 3
        getline(ss, authorCreator, ',');      // column 4
        getline(ss, category, ',');           // column 5
        getline(ss, totalSTR, ',');           // column 6
        getline(ss, availableSTR, ',');       // column 7
        getline(ss, availabilityStatus, ','); // column 8

        int id = stoi(idSTR);
        int totalCopies = stoi(totalSTR);

        if (type == "book")
        {
            string ISBN, publisher, yearSTR, skip;

            getline(ss, ISBN, ',');      // column 9
            getline(ss, publisher, ','); // column 10
            getline(ss, yearSTR, ',');   // column 11
            getline(ss, skip, ',');      // column 12 — blank (director)
            getline(ss, skip, ',');      // column 13 — blank (durationMinutes)
            getline(ss, skip, ',');      // column 14 — blank (genre)
            getline(ss, skip, ',');      // column 15 — blank (narrator)
            getline(ss, skip, ',');      // column 16 — blank (format)
            getline(ss, skip, ',');      // column 17 — blank (volumeNumber)
            getline(ss, skip, ',');      // column 18 — blank (issueNumber)
            getline(ss, skip, ',');      // column 19 — blank (publicationDate)
            getline(ss, skip, ',');      // column 20 — blank (editionDate)
            getline(ss, skip, ',');      // column 21 — blank (region)

            LibraryResource *r = new Book(id, title, authorCreator, category, totalCopies, ISBN, publisher, stoi(yearSTR));
            resources.push_back(r);
        }
        else if (type == "dvd")
        {
            string skip, director, durationSTR, genre;

            getline(ss, skip, ',');        // column 9  — blank (ISBN)
            getline(ss, skip, ',');        // column 10 — blank (publisher)
            getline(ss, skip, ',');        // column 11 — blank (yearPublished)
            getline(ss, director, ',');    // column 12
            getline(ss, durationSTR, ','); // column 13
            getline(ss, genre, ',');       // column 14

            LibraryResource *r = new DVD(id, title, director, category, totalCopies, stoi(durationSTR), genre);
            resources.push_back(r);
        }
        else if (type == "audiobook")
        {
            string skip, narrator, durationSTR, format;

            getline(ss, skip, ',');        // column 9  — blank (ISBN)
            getline(ss, skip, ',');        // column 10 — blank (publisher)
            getline(ss, skip, ',');        // column 11 — blank (yearPublished)
            getline(ss, skip, ',');        // column 12 — blank (director)
            getline(ss, durationSTR, ','); // column 13
            getline(ss, skip, ',');        // column 14 — blank (genre)
            getline(ss, narrator, ',');    // column 15
            getline(ss, format, ',');      // column 16

            LibraryResource *r = new AudioBook(id, title, authorCreator, category, totalCopies, narrator, stoi(durationSTR), format);
            resources.push_back(r);
        }
        else if (type == "magazine")
        {
            string skip, publisher, volumeSTR, issueSTR, publicationDate;

            getline(ss, skip, ',');            // column 9  — blank (ISBN)
            getline(ss, publisher, ',');       // column 10
            getline(ss, skip, ',');            // column 11 — blank (yearPublished)
            getline(ss, skip, ',');            // column 12 — blank (director)
            getline(ss, skip, ',');            // column 13 — blank (durationMinutes)
            getline(ss, skip, ',');            // column 14 — blank (genre)
            getline(ss, skip, ',');            // column 15 — blank (narrator)
            getline(ss, skip, ',');            // column 16 — blank (format)
            getline(ss, volumeSTR, ',');       // column 17
            getline(ss, issueSTR, ',');        // column 18
            getline(ss, publicationDate, ','); // column 19

            LibraryResource *r = new Magazine(id, title, publisher, category, totalCopies, stoi(volumeSTR), stoi(issueSTR), publicationDate);
            resources.push_back(r);
        }
        else if (type == "newspaper")
        {
            string skip, publisher, editionDate, region;

            getline(ss, skip, ',');        // column 9  — blank (ISBN)
            getline(ss, publisher, ',');   // column 10
            getline(ss, skip, ',');        // column 11 — blank (yearPublished)
            getline(ss, skip, ',');        // column 12 — blank (director)
            getline(ss, skip, ',');        // column 13 — blank (durationMinutes)
            getline(ss, skip, ',');        // column 14 — blank (genre)
            getline(ss, skip, ',');        // column 15 — blank (narrator)
            getline(ss, skip, ',');        // column 16 — blank (format)
            getline(ss, skip, ',');        // column 17 — blank (volumeNumber)
            getline(ss, skip, ',');        // column 18 — blank (issueNumber)
            getline(ss, skip, ',');        // column 19 — blank (publicationDate)
            getline(ss, editionDate, ','); // column 20
            getline(ss, region, ',');      // column 21

            LibraryResource *r = new Newspaper(id, title, publisher, category, totalCopies, editionDate, region);
            resources.push_back(r);
        }
    }

    readFile.close();
}

// rewrites entire CSV with current vector data — call after any modification
void SaveResourcesToFile(const string &filename, vector<LibraryResource *> &resources)
{
    ofstream writeFile(filename);

    if (!writeFile.is_open())
    {
        cout << "Error: Could not open " << filename << endl;
        return;
    }

    // write header row
    writeFile << "resourceID,type,title,authorCreator,category,totalCopies,availableCopies,"
              << "availabilityStatus,ISBN,publisher,yearPublished,director,durationMinutes,"
              << "genre,narrator,format,volumeNumber,issueNumber,publicationDate,editionDate,region\n";

    for (auto res : resources)
    {
        // write common columns
        writeFile << res->getResourceID() << ","          // column 1
                  << res->getType() << ","                // column 2
                  << res->getTitle() << ","               // column 3
                  << res->getAuthorCreator() << ","       // column 4
                  << res->getCategory() << ","            // column 5
                  << res->getTotalCopies() << ","         // column 6
                  << res->getAvailableCopies() << ","     // column 7
                  << res->getAvailabilityStatus() << ","; // column 8

        if (res->getType() == "book")
        {
            Book *b = dynamic_cast<Book *>(res);
            writeFile << b->getISBN() << ","          // column 9
                      << b->getPublisher() << ","     // column 10
                      << b->getYearPublished() << "," // column 11
                      << ","                          // column 12 blank
                      << ","                          // column 13 blank
                      << ","                          // column 14 blank
                      << ","                          // column 15 blank
                      << ","                          // column 16 blank
                      << ","                          // column 17 blank
                      << ","                          // column 18 blank
                      << ","                          // column 19 blank
                      << ","                          // column 20 blank
                      << "\n";                        // column 21 blank
        }
        else if (res->getType() == "dvd")
        {
            DVD *d = dynamic_cast<DVD *>(res);
            writeFile << ","                     // column 9  blank
                      << ","                     // column 10 blank
                      << ","                     // column 11 blank
                      << d->getDirector() << "," // column 12
                      << d->getDuration() << "," // column 13
                      << d->getGenre() << ","    // column 14
                      << ","                     // column 15 blank
                      << ","                     // column 16 blank
                      << ","                     // column 17 blank
                      << ","                     // column 18 blank
                      << ","                     // column 19 blank
                      << ","                     // column 20 blank
                      << "\n";                   // column 21 blank
        }
        else if (res->getType() == "audiobook")
        {
            AudioBook *a = dynamic_cast<AudioBook *>(res);
            writeFile << ","                     // column 9  blank
                      << ","                     // column 10 blank
                      << ","                     // column 11 blank
                      << ","                     // column 12 blank
                      << a->getDuration() << "," // column 13
                      << ","                     // column 14 blank
                      << a->getNarrator() << "," // column 15
                      << a->getFormat() << ","   // column 16
                      << ","                     // column 17 blank
                      << ","                     // column 18 blank
                      << ","                     // column 19 blank
                      << ","                     // column 20 blank
                      << "\n";                   // column 21 blank
        }
        else if (res->getType() == "magazine")
        {
            Magazine *m = dynamic_cast<Magazine *>(res);
            writeFile << ","                            // column 9  blank
                      << m->getPublisher() << ","       // column 10
                      << ","                            // column 11 blank
                      << ","                            // column 12 blank
                      << ","                            // column 13 blank
                      << ","                            // column 14 blank
                      << ","                            // column 15 blank
                      << ","                            // column 16 blank
                      << m->getVolumeNumber() << ","    // column 17
                      << m->getIssueNumber() << ","     // column 18
                      << m->getPublicationDate() << "," // column 19
                      << ","                            // column 20 blank
                      << "\n";                          // column 21 blank
        }
        else if (res->getType() == "newspaper")
        {
            Newspaper *n = dynamic_cast<Newspaper *>(res);
            writeFile << ","                        // column 9  blank
                      << n->getPublisher() << ","   // column 10
                      << ","                        // column 11 blank
                      << ","                        // column 12 blank
                      << ","                        // column 13 blank
                      << ","                        // column 14 blank
                      << ","                        // column 15 blank
                      << ","                        // column 16 blank
                      << ","                        // column 17 blank
                      << ","                        // column 18 blank
                      << ","                        // column 19 blank
                      << n->getEditionDate() << "," // column 20
                      << n->getRegion() << "\n";    // column 21
        }
    }

    writeFile.close();
}

// scans CSV for highest existing ID, returns max + 1 for new registration
int generateNewUserID(const string &filename)
{
    ifstream readFile(filename);
    int maxID = 0;
    string line;

    getline(readFile, line); // skip header

    while (getline(readFile, line))
    {
        stringstream ss(line);
        string idSTR;
        getline(ss, idSTR, ',');
        int id = stoi(idSTR);
        if (id > maxID)
            maxID = id;
    }

    readFile.close();
    return maxID + 1;
}

// same logic as generateNewUserID but for resources CSV
int generateNewResourceID(const string &filename)
{
    ifstream readFile(filename);
    int maxID = 0;
    string line;

    getline(readFile, line); // skip header

    while (getline(readFile, line))
    {
        stringstream ss(line);
        string idSTR;
        getline(ss, idSTR, ',');
        int id = stoi(idSTR);
        if (id > maxID)
            maxID = id;
    }

    readFile.close();
    return maxID + 1;
}