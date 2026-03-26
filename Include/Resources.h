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
    string type; // "book", "dvd", "audiobook", "magazine", "newspaper"
    string title;
    string authorCreator;
    string category;
    string availabilityStatus; // "Available", "Partially Available", "Borrowed"
    int totalCopies;
    int availableCopies;
    bool isDeleted; // marks resource as deleted without removing from vector

public:
    // default constructor
    LibraryResource();

    // parameterized constructor
    LibraryResource(int ID, string type, string title, string authorCreator,
                    string category, int totalCopies);

    // getters
    int getResourceID() const;
    string getType() const;
    string getTitle() const;
    string getAuthorCreator() const;
    string getCategory() const;
    string getAvailabilityStatus() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    // setters
    void setTitle(string t);
    void setAuthorCreator(string a);
    void setCategory(string c);
    void setTotalCopies(int t);

    // updates status based on current available copies
    void updateStatus();

    // checks availability before adding resource to the available list
    bool isAvailable();

    // decreases available copies by 1 if any copy exists, returns false if none available
    bool borrowResource();

    // prevents available copies from exceeding total copies on return
    void returnResource();

    // each derived class must define its own display
    virtual void displayInfo() = 0;

    // For marking resource as deleted
    void markDeleted();        // marks resource as deleted
    bool getIsDeleted() const; // returns deleted status

    virtual ~LibraryResource() {}
};

// Derived class for physical books
class Book : public LibraryResource
{
private:
    string ISBN;
    string publisher;
    int yearPublished;

public:
    // default constructor
    Book();

    // parameterized constructor
    Book(int ID, string title, string author, string category,
         int totalCopies, string ISBN, string publisher, int year);

    // getters
    string getISBN() const;
    string getPublisher() const;
    int getYearPublished() const;

    // setters
    void setISBN(string i);
    void setPublisher(string p);
    void setYear(int y);

    void displayInfo() override;
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
    DVD();

    // parameterized constructor
    DVD(int ID, string title, string director, string category,
        int totalCopies, int duration, string genre);

    // getters
    string getDirector() const;
    int getDuration() const;
    string getGenre() const;

    // setters
    void setDirector(string d);
    void setDuration(int d);
    void setGenre(string g);

    void displayInfo() override;
};

// Derived class for narrated audio versions of books
class AudioBook : public LibraryResource
{
private:
    string narrator;
    int durationMinutes;
    string format; // MP3, CD etc.

public:
    // default constructor
    AudioBook();

    // parameterized constructor
    AudioBook(int ID, string title, string author, string category,
              int totalCopies, string narrator, int duration, string format);

    // getters
    string getNarrator() const;
    int getDuration() const;
    string getFormat() const;

    // setters
    void setNarrator(string n);
    void setDuration(int d);
    void setFormat(string f);

    void displayInfo() override;
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
    Magazine();

    // parameterized constructor
    Magazine(int ID, string title, string publisher, string category,
             int totalCopies, int volume, int issue, string pubDate);

    // getters
    int getVolumeNumber() const;
    int getIssueNumber() const;
    string getPublisher() const;
    string getPublicationDate() const;

    // setters
    void setVolumeNumber(int v);
    void setIssueNumber(int i);
    void setPublisher(string p);
    void setPublicationDate(string d);

    void displayInfo() override;
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
    Newspaper();

    // parameterized constructor
    Newspaper(int ID, string title, string publisher, string category,
              int totalCopies, string editionDate, string region);

    // getters
    string getEditionDate() const;
    string getRegion() const;
    string getPublisher() const;

    // setters
    void setEditionDate(string d);
    void setRegion(string r);
    void setPublisher(string p);

    void displayInfo() override;
};

#endif // RESOURCE_H