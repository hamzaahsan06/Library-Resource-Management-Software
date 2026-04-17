#include "FileHandler.h"
#include <iostream>
using namespace std;

// CSV column order:
// userID,type,username,password,name,address,balance,isDeleted,department,rollNo,designation,position,membershipLevel

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
        string idSTR, type, username, password, name, address, balSTR, isDeletedSTR;

        getline(ss, idSTR,        ','); // column 1
        getline(ss, type,         ','); // column 2
        getline(ss, username,     ','); // column 3
        getline(ss, password,     ','); // column 4
        getline(ss, name,         ','); // column 5
        getline(ss, address,      ','); // column 6
        getline(ss, balSTR,       ','); // column 7
        getline(ss, isDeletedSTR, ','); // column 8 — isDeleted flag (0 = active, 1 = deleted)

        int id       = stoi(idSTR);
        double bal   = stod(balSTR);
        bool deleted = (isDeletedSTR == "1"); // convert to bool

        User *u = nullptr;

        if (type == "student")
        {
            string department, rollSTR;
            getline(ss, department, ','); // column 9
            getline(ss, rollSTR,    ','); // column 10

            u = new Student(id, username, password, name, address, bal, department, stoi(rollSTR));
        }
        else if (type == "teacher")
        {
            string skip, department, designation;
            getline(ss, department,  ','); // column 9
            getline(ss, skip,        ','); // column 10 — blank (rollNo)
            getline(ss, designation, ','); // column 11

            u = new Teacher(id, username, password, name, address, bal, department, designation);
        }
        else if (type == "staff")
        {
            string skip, position;
            getline(ss, skip,     ','); // column 9  — blank (department)
            getline(ss, skip,     ','); // column 10 — blank (rollNo)
            getline(ss, skip,     ','); // column 11 — blank (designation)
            getline(ss, position, ','); // column 12

            u = new Staff(id, username, password, name, address, bal, position);
        }
        else if (type == "premium")
        {
            string skip, membershipLevel;
            getline(ss, skip,            ','); // column 9  — blank (department)
            getline(ss, skip,            ','); // column 10 — blank (rollNo)
            getline(ss, skip,            ','); // column 11 — blank (designation)
            getline(ss, skip,            ','); // column 12 — blank (position)
            getline(ss, membershipLevel, ','); // column 13

            u = new PremiumMember(id, username, password, name, address, bal, membershipLevel);
        }
        else if (type == "admin")
        {
            // no additional attributes
            u = new Admin(id, username, password, name, address, bal);
        }

        // push into vector and restore deleted status if marked in CSV
        if (u != nullptr)
        {
            if (deleted)
                u->markDeleted(); // restore deleted status from CSV
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
              << "isDeleted,department,rollNo,designation,position,membershipLevel\n";

    for (auto user : users)
    {
        // write common columns
        writeFile << user->getUserID()    << ","  // column 1
                  << user->getType()      << ","  // column 2
                  << user->getUsername()  << ","  // column 3
                  << user->getPassword()  << ","  // column 4
                  << user->getName()      << ","  // column 5
                  << user->getAddress()   << ","  // column 6
                  << user->getBalance()   << ","  // column 7
                  << user->getIsDeleted() << ","; // column 8 — write 1 if deleted, 0 if active

        if (user->getType() == "student")
        {
            Student *s = dynamic_cast<Student *>(user);
            writeFile << s->getDepartment() << "," // column 9
                      << s->getRollNo()     << "," // column 10
                      << ",,"               << "\n"; // columns 11,12,13 blank
        }
        else if (user->getType() == "teacher")
        {
            Teacher *t = dynamic_cast<Teacher *>(user);
            writeFile << t->getDepartment()  << ","  // column 9
                      << ","                         // column 10 blank
                      << t->getDesignation() << ","  // column 11
                      << ","                << "\n"; // columns 12,13 blank
        }
        else if (user->getType() == "staff")
        {
            Staff *s = dynamic_cast<Staff *>(user);
            writeFile << ",,,"                   // columns 9,10,11 blank
                      << s->getPosition() << "," // column 12
                      << "\n";                   // column 13 blank
        }
        else if (user->getType() == "premium")
        {
            PremiumMember *p = dynamic_cast<PremiumMember *>(user);
            writeFile << ",,,,"                           // columns 9,10,11,12 blank
                      << p->getMembershipLevel() << "\n"; // column 13
        }
        else if (user->getType() == "admin")
        {
            writeFile << ",,,,\n"; // columns 9-13 all blank
        }
    }

    writeFile.close();
}

// CSV column order:
// resourceID,type,title,authorCreator,category,totalCopies,availableCopies,availabilityStatus,isDeleted,
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
        string idSTR, type, title, authorCreator, category, totalSTR, availableSTR, availabilityStatus, isDeletedSTR;

        getline(ss, idSTR,              ','); // column 1
        getline(ss, type,               ','); // column 2
        getline(ss, title,              ','); // column 3
        getline(ss, authorCreator,      ','); // column 4
        getline(ss, category,           ','); // column 5
        getline(ss, totalSTR,           ','); // column 6
        getline(ss, availableSTR,       ','); // column 7
        getline(ss, availabilityStatus, ','); // column 8
        getline(ss, isDeletedSTR,       ','); // column 9 — isDeleted flag (0 = active, 1 = deleted)

        int id          = stoi(idSTR);
        int totalCopies = stoi(totalSTR);
        bool deleted    = (isDeletedSTR == "1"); // convert to bool

        LibraryResource *r = nullptr;

        if (type == "book")
        {
            string ISBN, publisher, yearSTR, skip;

            getline(ss, ISBN,      ','); // column 10
            getline(ss, publisher, ','); // column 11
            getline(ss, yearSTR,   ','); // column 12
            getline(ss, skip, ',');      // column 13 — blank (director)
            getline(ss, skip, ',');      // column 14 — blank (durationMinutes)
            getline(ss, skip, ',');      // column 15 — blank (genre)
            getline(ss, skip, ',');      // column 16 — blank (narrator)
            getline(ss, skip, ',');      // column 17 — blank (format)
            getline(ss, skip, ',');      // column 18 — blank (volumeNumber)
            getline(ss, skip, ',');      // column 19 — blank (issueNumber)
            getline(ss, skip, ',');      // column 20 — blank (publicationDate)
            getline(ss, skip, ',');      // column 21 — blank (editionDate)
            getline(ss, skip, ',');      // column 22 — blank (region)

            r = new Book(id, title, authorCreator, category, totalCopies, ISBN, publisher, stoi(yearSTR));
        }
        else if (type == "dvd")
        {
            string skip, director, durationSTR, genre;

            getline(ss, skip, ',');        // column 10 — blank (ISBN)
            getline(ss, skip, ',');        // column 11 — blank (publisher)
            getline(ss, skip, ',');        // column 12 — blank (yearPublished)
            getline(ss, director,    ','); // column 13
            getline(ss, durationSTR, ','); // column 14
            getline(ss, genre,       ','); // column 15

            r = new DVD(id, title, director, category, totalCopies, stoi(durationSTR), genre);
        }
        else if (type == "audiobook")
        {
            string skip, narrator, durationSTR, format;

            getline(ss, skip, ',');        // column 10 — blank (ISBN)
            getline(ss, skip, ',');        // column 11 — blank (publisher)
            getline(ss, skip, ',');        // column 12 — blank (yearPublished)
            getline(ss, skip, ',');        // column 13 — blank (director)
            getline(ss, durationSTR, ','); // column 14
            getline(ss, skip, ',');        // column 15 — blank (genre)
            getline(ss, narrator,    ','); // column 16
            getline(ss, format,      ','); // column 17

            r = new AudioBook(id, title, authorCreator, category, totalCopies, narrator, stoi(durationSTR), format);
        }
        else if (type == "magazine")
        {
            string skip, publisher, volumeSTR, issueSTR, publicationDate;

            getline(ss, skip, ',');            // column 10 — blank (ISBN)
            getline(ss, publisher,       ','); // column 11
            getline(ss, skip, ',');            // column 12 — blank (yearPublished)
            getline(ss, skip, ',');            // column 13 — blank (director)
            getline(ss, skip, ',');            // column 14 — blank (durationMinutes)
            getline(ss, skip, ',');            // column 15 — blank (genre)
            getline(ss, skip, ',');            // column 16 — blank (narrator)
            getline(ss, skip, ',');            // column 17 — blank (format)
            getline(ss, volumeSTR,       ','); // column 18
            getline(ss, issueSTR,        ','); // column 19
            getline(ss, publicationDate, ','); // column 20

            r = new Magazine(id, title, publisher, category, totalCopies, stoi(volumeSTR), stoi(issueSTR), publicationDate);
        }
        else if (type == "newspaper")
        {
            string skip, publisher, editionDate, region;

            getline(ss, skip, ',');        // column 10 — blank (ISBN)
            getline(ss, publisher,   ','); // column 11
            getline(ss, skip, ',');        // column 12 — blank (yearPublished)
            getline(ss, skip, ',');        // column 13 — blank (director)
            getline(ss, skip, ',');        // column 14 — blank (durationMinutes)
            getline(ss, skip, ',');        // column 15 — blank (genre)
            getline(ss, skip, ',');        // column 16 — blank (narrator)
            getline(ss, skip, ',');        // column 17 — blank (format)
            getline(ss, skip, ',');        // column 18 — blank (volumeNumber)
            getline(ss, skip, ',');        // column 19 — blank (issueNumber)
            getline(ss, skip, ',');        // column 20 — blank (publicationDate)
            getline(ss, editionDate, ','); // column 21
            getline(ss, region,      ','); // column 22

            r = new Newspaper(id, title, publisher, category, totalCopies, editionDate, region);
        }

        // push into vector and restore deleted status if marked in CSV
        if (r != nullptr)
        {
            if (deleted)
                r->markDeleted(); // restore deleted status from CSV
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
              << "availabilityStatus,isDeleted,ISBN,publisher,yearPublished,director,durationMinutes,"
              << "genre,narrator,format,volumeNumber,issueNumber,publicationDate,editionDate,region\n";

    for (auto res : resources)
    {
        // write common columns
        writeFile << res->getResourceID()         << ","  // column 1
                  << res->getType()               << ","  // column 2
                  << res->getTitle()              << ","  // column 3
                  << res->getAuthorCreator()      << ","  // column 4
                  << res->getCategory()           << ","  // column 5
                  << res->getTotalCopies()        << ","  // column 6
                  << res->getAvailableCopies()    << ","  // column 7
                  << res->getAvailabilityStatus() << ","  // column 8
                  << res->getIsDeleted()          << ","; // column 9 — write 1 if deleted, 0 if active

        if (res->getType() == "book")
        {
            Book *b = dynamic_cast<Book *>(res);
            writeFile << b->getISBN()          << ","  // column 10
                      << b->getPublisher()     << ","  // column 11
                      << b->getYearPublished() << ","  // column 12
                      << ","                           // column 13 blank
                      << ","                           // column 14 blank
                      << ","                           // column 15 blank
                      << ","                           // column 16 blank
                      << ","                           // column 17 blank
                      << ","                           // column 18 blank
                      << ","                           // column 19 blank
                      << ","                           // column 20 blank
                      << ","                           // column 21 blank
                      << "\n";                         // column 22 blank
        }
        else if (res->getType() == "dvd")
        {
            DVD *d = dynamic_cast<DVD *>(res);
            writeFile << ","                     // column 10 blank
                      << ","                     // column 11 blank
                      << ","                     // column 12 blank
                      << d->getDirector() << "," // column 13
                      << d->getDuration() << "," // column 14
                      << d->getGenre()    << "," // column 15
                      << ","                     // column 16 blank
                      << ","                     // column 17 blank
                      << ","                     // column 18 blank
                      << ","                     // column 19 blank
                      << ","                     // column 20 blank
                      << ","                     // column 21 blank
                      << "\n";                   // column 22 blank
        }
        else if (res->getType() == "audiobook")
        {
            AudioBook *a = dynamic_cast<AudioBook *>(res);
            writeFile << ","                     // column 10 blank
                      << ","                     // column 11 blank
                      << ","                     // column 12 blank
                      << ","                     // column 13 blank
                      << a->getDuration() << "," // column 14
                      << ","                     // column 15 blank
                      << a->getNarrator() << "," // column 16
                      << a->getFormat()   << "," // column 17
                      << ","                     // column 18 blank
                      << ","                     // column 19 blank
                      << ","                     // column 20 blank
                      << ","                     // column 21 blank
                      << "\n";                   // column 22 blank
        }
        else if (res->getType() == "magazine")
        {
            Magazine *m = dynamic_cast<Magazine *>(res);
            writeFile << ","                            // column 10 blank
                      << m->getPublisher()      << ","  // column 11
                      << ","                            // column 12 blank
                      << ","                            // column 13 blank
                      << ","                            // column 14 blank
                      << ","                            // column 15 blank
                      << ","                            // column 16 blank
                      << ","                            // column 17 blank
                      << m->getVolumeNumber()   << ","  // column 18
                      << m->getIssueNumber()    << ","  // column 19
                      << m->getPublicationDate()<< ","  // column 20
                      << ","                            // column 21 blank
                      << "\n";                          // column 22 blank
        }
        else if (res->getType() == "newspaper")
        {
            Newspaper *n = dynamic_cast<Newspaper *>(res);
            writeFile << ","                        // column 10 blank
                      << n->getPublisher()  << ","  // column 11
                      << ","                        // column 12 blank
                      << ","                        // column 13 blank
                      << ","                        // column 14 blank
                      << ","                        // column 15 blank
                      << ","                        // column 16 blank
                      << ","                        // column 17 blank
                      << ","                        // column 18 blank
                      << ","                        // column 19 blank
                      << ","                        // column 20 blank
                      << n->getEditionDate()<< ","  // column 21
                      << n->getRegion()     << "\n";// column 22
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
