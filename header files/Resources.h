#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <string>
using namespace std;

// Base class for all library resources
class LibraryResource
{
protected:
    int resourceID;
    string title;
    string authorCreator;
    string category;
    string availabilityStatus; // "Available", "Partially available", "Borrowed"
    int totalCopies;
    int availableCopies;
    static int idCounter; // shared counter to auto-generate unique IDs

public:
    // default constructor
    LibraryResource()
    {
        resourceID = ++idCounter;
        title = "Unknown";
        authorCreator = "Unknown";
        category = "General";
        totalCopies = 1;
        availableCopies = 1;
        availabilityStatus = "Available";
    }

    // parameterized constructor, ID still auto-assigned
    LibraryResource(string title, string authorCreator, string category, int totalCopies)
    {
        resourceID = ++idCounter;
        this->title = title;
        this->authorCreator = authorCreator;
        this->category = category;
        this->totalCopies = totalCopies;
        this->availableCopies = totalCopies; // initially all copies are available
        availabilityStatus = "Available";
    }

    // getters
    int getResourceID() const { return resourceID; }
    string getTitle() const { return title; }
    string getAuthorCreator() const { return authorCreator; }
    string getCategory() const { return category; }
    string getAvailabilityStatus() const { return availabilityStatus; }
    int getTotalCopies() const { return totalCopies; }
    int getAvailableCopies() const { return availableCopies; }

    // setters
    void setTitle(string t) { title = t; }
    void setAuthorCreator(string a) { authorCreator = a; }
    void setCategory(string c) { category = c; }
    void setTotalCopies(int t) { totalCopies = t; }

    // updates status based on current available copies
    void updateStatus()
    {
        if (availableCopies == 0)
            availabilityStatus = "Borrowed";
        else if (availableCopies < totalCopies)
            availabilityStatus = "Partially Available";
        else
            availabilityStatus = "Available";
    }

    // checks availability before adding resource to the available list
    bool isAvailable()
    {
        return availableCopies > 0;
    }

    // decreases available copies by 1 if any copy exists, returns false if none available
    bool borrowResource()
    {
        if (availableCopies > 0)
        {
            availableCopies--;
            updateStatus();
            return true;
        }
        return false;
    }

    // prevents available copies from exceeding total copies on return
    void returnResource()
    {
        if (availableCopies < totalCopies)
        {
            availableCopies++;
            updateStatus();
        }
    }

    // each derived class must define its own display
    virtual void displayInfo() = 0;

    virtual ~LibraryResource() {}
};

// start IDs from 1
int LibraryResource::idCounter = 0;

// Derived class for physical books
class Book : public LibraryResource
{
private:
    string ISBN;
    string publisher;
    int yearPublished;

public:
    // default constructor
    Book() : LibraryResource()
    {
        ISBN = "000-0000000000";
        publisher = "Unknown";
        yearPublished = 0;
    }

    // parameterized constructor
    Book(string title, string author, string category,
         int totalCopies, string ISBN, string publisher, int year)
        : LibraryResource(title, author, category, totalCopies)
    {
        this->ISBN = ISBN;
        this->publisher = publisher;
        this->yearPublished = year;
    }

    // getters
    string getISBN() const { return ISBN; }
    string getPublisher() const { return publisher; }
    int getYearPublished() const { return yearPublished; }

    // setters
    void setISBN(string i) { ISBN = i; }
    void setPublisher(string p) { publisher = p; }
    void setYear(int y) { yearPublished = y; }

    void displayInfo() override
    {
        cout << "Resource ID  : " << resourceID << endl;
        cout << "Title        : " << title << endl;
        cout << "Author       : " << authorCreator << endl;
        cout << "Category     : " << category << endl;
        cout << "ISBN         : " << ISBN << endl;
        cout << "Publisher    : " << publisher << endl;
        cout << "Year         : " << yearPublished << endl;
        cout << "Total Copies : " << totalCopies << endl;
        cout << "Available    : " << availableCopies << endl;
        cout << "Status       : " << availabilityStatus << endl;
    }
};

// Derived class for DVD
class DVD : public LibraryResource
{
private:
    string director;
    int durationMinutes;
    string genre;

public:
    // default constructor
    DVD() : LibraryResource()
    {
        director = "Unknown";
        durationMinutes = 0;
        genre = "Unknown";
    }

    // parameterized constructor
    DVD(string title, string director, string category,
        int totalCopies, int duration, string genre)
        : LibraryResource(title, director, category, totalCopies)
    {
        this->director = director;
        this->durationMinutes = duration;
        this->genre = genre;
    }

    // getters
    string getDirector() const { return director; }
    int getDuration() const { return durationMinutes; }
    string getGenre() const { return genre; }

    // setters
    void setDirector(string d) { director = d; }
    void setDuration(int d) { durationMinutes = d; }
    void setGenre(string g) { genre = g; }

    void displayInfo() override
    {
        cout << "Resource ID  : " << resourceID << endl;
        cout << "Title        : " << title << endl;
        cout << "Director     : " << director << endl;
        cout << "Category     : " << category << endl;
        cout << "Genre        : " << genre << endl;
        cout << "Duration     : " << durationMinutes << " mins" << endl;
        cout << "Total Copies : " << totalCopies << endl;
        cout << "Available    : " << availableCopies << endl;
        cout << "Status       : " << availabilityStatus << endl;
    }
};


// Derived class for periodical magazines
class Magazine : public LibraryResource
{
private:
    int volumeNumber;
    int issueNumber;
    string publisher;
    string publicationDate; // e.g. "May 2025"

public:
    // default constructor
    Magazine() : LibraryResource()
    {
        volumeNumber = 1;
        issueNumber = 1;
        publisher = "Unknown";
        publicationDate = "Unknown";
    }

    // parameterized constructor
    Magazine(string title, string publisher, string category,
             int totalCopies, int volume, int issue, string pubDate)
        : LibraryResource(title, publisher, category, totalCopies)
    {
        this->volumeNumber = volume;
        this->issueNumber = issue;
        this->publisher = publisher;
        this->publicationDate = pubDate;
    }

    // getters
    int getVolumeNumber() const { return volumeNumber; }
    int getIssueNumber() const { return issueNumber; }
    string getPublisher() const { return publisher; }
    string getPublicationDate() const { return publicationDate; }

    // setters
    void setVolumeNumber(int v) { volumeNumber = v; }
    void setIssueNumber(int i) { issueNumber = i; }
    void setPublisher(string p) { publisher = p; }
    void setPublicationDate(string d) { publicationDate = d; }

    void displayInfo() override
    {
        cout << "Resource ID  : " << resourceID << endl;
        cout << "Title        : " << title << endl;
        cout << "Publisher    : " << publisher << endl;
        cout << "Category     : " << category << endl;
        cout << "Volume No.   : " << volumeNumber << endl;
        cout << "Issue No.    : " << issueNumber << endl;
        cout << "Pub. Date    : " << publicationDate << endl;
        cout << "Total Copies : " << totalCopies << endl;
        cout << "Available    : " << availableCopies << endl;
        cout << "Status       : " << availabilityStatus << endl;
    }
};

// Derived class for daily or weekly newspapers
class Newspaper : public LibraryResource
{
private:
    string editionDate;
    string region;
    string publisher;

public:
    // default constructor
    Newspaper() : LibraryResource()
    {
        editionDate = "Unknown";
        region = "Unknown";
        publisher = "Unknown";
    }

    // parameterized constructor
    Newspaper(string title, string publisher, string category,
              int totalCopies, string editionDate, string region)
        : LibraryResource(title, publisher, category, totalCopies)
    {
        this->editionDate = editionDate;
        this->region = region;
        this->publisher = publisher;
    }

    // getters
    string getEditionDate() const { return editionDate; }
    string getRegion() const { return region; }
    string getPublisher() const { return publisher; }

    // setters
    void setEditionDate(string d) { editionDate = d; }
    void setRegion(string r) { region = r; }
    void setPublisher(string p) { publisher = p; }

    void displayInfo() override
    {
        cout << "Resource ID  : " << resourceID << endl;
        cout << "Title        : " << title << endl;
        cout << "Publisher    : " << publisher << endl;
        cout << "Category     : " << category << endl;
        cout << "Edition Date : " << editionDate << endl;
        cout << "Region       : " << region << endl;
        cout << "Total Copies : " << totalCopies << endl;
        cout << "Available    : " << availableCopies << endl;
        cout << "Status       : " << availabilityStatus << endl;
    }
};

#endif // RESOURCE_H