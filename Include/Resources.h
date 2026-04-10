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
    string type;
    string title;
    string authorCreator;
    string category;
    string availabilityStatus;
    int totalCopies;
    int availableCopies;
    bool isDeleted;

public:
    LibraryResource();

    LibraryResource(int ID, string type, string title, string authorCreator,
                    string category, int totalCopies);

    // ---------- getters (FIXED CONST) ----------
    int getResourceID() const;
    string getType() const;
    string getTitle() const;
    string getAuthorCreator() const;
    string getCategory() const;
    string getAvailabilityStatus() const;
    int getTotalCopies() const;
    int getAvailableCopies() const;

    // ---------- setters ----------
    void setTitle(string t);
    void setAuthorCreator(string a);
    void setCategory(string c);
    void setTotalCopies(int t);

    // ---------- logic ----------
    void updateStatus();

    bool isAvailable() const;   //  FIXED CONST

    bool borrowResource();
    void returnResource();

    virtual void displayInfo() = 0;

    // ---------- added for compatibility with Library.cpp ----------
    void display() const { displayInfo(); }  //  IMPORTANT FIX

    void markDeleted();
    bool getIsDeleted() const;

    virtual ~LibraryResource() {}
};

// ---------------- BOOK ----------------
class Book : public LibraryResource
{
private:
    string ISBN;
    string publisher;
    int yearPublished;

public:
    Book();

    Book(int ID, string title, string author, string category,
         int totalCopies, string ISBN, string publisher, int year);

    string getISBN() const;
    string getPublisher() const;
    int getYearPublished() const;

    void setISBN(string i);
    void setPublisher(string p);
    void setYear(int y);

    void displayInfo() override;
};

// ---------------- DVD ----------------
class DVD : public LibraryResource
{
private:
    string director;
    int durationMinutes;
    string genre;

public:
    DVD();

    DVD(int ID, string title, string director, string category,
        int totalCopies, int duration, string genre);

    string getDirector() const;
    int getDuration() const;
    string getGenre() const;

    void setDirector(string d);
    void setDuration(int d);
    void setGenre(string g);

    void displayInfo() override;
};

// ---------------- AUDIOBOOK ----------------
class AudioBook : public LibraryResource
{
private:
    string narrator;
    int durationMinutes;
    string format;

public:
    AudioBook();

    AudioBook(int ID, string title, string author, string category,
              int totalCopies, string narrator, int duration, string format);

    string getNarrator() const;
    int getDuration() const;
    string getFormat() const;

    void setNarrator(string n);
    void setDuration(int d);
    void setFormat(string f);

    void displayInfo() override;
};

// ---------------- MAGAZINE ----------------
class Magazine : public LibraryResource
{
private:
    int volumeNumber;
    int issueNumber;
    string publisher;
    string publicationDate;

public:
    Magazine();

    Magazine(int ID, string title, string publisher, string category,
             int totalCopies, int volume, int issue, string pubDate);

    int getVolumeNumber() const;
    int getIssueNumber() const;
    string getPublisher() const;
    string getPublicationDate() const;

    void setVolumeNumber(int v);
    void setIssueNumber(int i);
    void setPublisher(string p);
    void setPublicationDate(string d);

    void displayInfo() override;
};

// ---------------- NEWSPAPER ----------------
class Newspaper : public LibraryResource
{
private:
    string editionDate;
    string region;
    string publisher;

public:
    Newspaper();

    Newspaper(int ID, string title, string publisher, string category,
              int totalCopies, string editionDate, string region);

    string getEditionDate() const;
    string getRegion() const;
    string getPublisher() const;

    void setEditionDate(string d);
    void setRegion(string r);
    void setPublisher(string p);

    void displayInfo() override;
};

#endif
