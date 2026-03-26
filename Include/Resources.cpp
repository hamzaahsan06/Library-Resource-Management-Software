#include "Resources.h"
#include <iostream>
using namespace std;

// ===================== LibraryResource =====================

// default constructor
LibraryResource::LibraryResource()
{
    resourceID = 0;
    type = "unknown";
    title = "Unknown";
    authorCreator = "Unknown";
    category = "General";
    totalCopies = 1;
    availableCopies = 1;
    availabilityStatus = "Available";
    isDeleted = false;
}

// parameterized constructor
LibraryResource::LibraryResource(int ID, string type, string title,
                                 string authorCreator, string category, int totalCopies)
{
    resourceID = ID;
    this->type = type;
    this->title = title;
    this->authorCreator = authorCreator;
    this->category = category;
    this->totalCopies = totalCopies;
    this->availableCopies = totalCopies; // initially all copies available
    availabilityStatus = "Available";
    isDeleted = false;
}

// getters
int LibraryResource::getResourceID() const { return resourceID; }
string LibraryResource::getType() const { return type; }
string LibraryResource::getTitle() const { return title; }
string LibraryResource::getAuthorCreator() const { return authorCreator; }
string LibraryResource::getCategory() const { return category; }
string LibraryResource::getAvailabilityStatus() const { return availabilityStatus; }
int LibraryResource::getTotalCopies() const { return totalCopies; }
int LibraryResource::getAvailableCopies() const { return availableCopies; }

// setters
void LibraryResource::setTitle(string t) { title = t; }
void LibraryResource::setAuthorCreator(string a) { authorCreator = a; }
void LibraryResource::setCategory(string c) { category = c; }
void LibraryResource::setTotalCopies(int t) { totalCopies = t; }

// updates status based on current available copies
void LibraryResource::updateStatus()
{
    if (availableCopies == 0)
        availabilityStatus = "Borrowed";
    else if (availableCopies < totalCopies)
        availabilityStatus = "Partially Available";
    else
        availabilityStatus = "Available";
}

// checks availability before adding resource to the available list
bool LibraryResource::isAvailable()
{
    return availableCopies > 0;
}

// decreases available copies by 1 if any copy exists, returns false if none available
bool LibraryResource::borrowResource()
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
void LibraryResource::returnResource()
{
    if (availableCopies < totalCopies)
    {
        availableCopies++;
        updateStatus();
    }
}

// For marking resource as deleted
void LibraryResource::markDeleted() { isDeleted = true; }
bool LibraryResource::getIsDeleted() const { return isDeleted; }

// ===================== Book =====================

// default constructor
Book::Book() : LibraryResource()
{
    type = "book";
    ISBN = "000-0000000000";
    publisher = "Unknown";
    yearPublished = 0;
}

// parameterized constructor
Book::Book(int ID, string title, string author, string category,
           int totalCopies, string ISBN, string publisher, int year)
    : LibraryResource(ID, "book", title, author, category, totalCopies)
{
    this->ISBN = ISBN;
    this->publisher = publisher;
    this->yearPublished = year;
}

// getters
string Book::getISBN() const { return ISBN; }
string Book::getPublisher() const { return publisher; }
int Book::getYearPublished() const { return yearPublished; }

// setters
void Book::setISBN(string i) { ISBN = i; }
void Book::setPublisher(string p) { publisher = p; }
void Book::setYear(int y) { yearPublished = y; }

void Book::displayInfo()
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

// ===================== DVD =====================

// default constructor
DVD::DVD() : LibraryResource()
{
    type = "dvd";
    director = "Unknown";
    durationMinutes = 0;
    genre = "Unknown";
}

// parameterized constructor
DVD::DVD(int ID, string title, string director, string category,
         int totalCopies, int duration, string genre)
    : LibraryResource(ID, "dvd", title, director, category, totalCopies)
{
    this->director = director;
    this->durationMinutes = duration;
    this->genre = genre;
}

// getters
string DVD::getDirector() const { return director; }
int DVD::getDuration() const { return durationMinutes; }
string DVD::getGenre() const { return genre; }

// setters
void DVD::setDirector(string d) { director = d; }
void DVD::setDuration(int d) { durationMinutes = d; }
void DVD::setGenre(string g) { genre = g; }

void DVD::displayInfo()
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

// ===================== AudioBook =====================

// default constructor
AudioBook::AudioBook() : LibraryResource()
{
    type = "audiobook";
    narrator = "Unknown";
    durationMinutes = 0;
    format = "MP3";
}

// parameterized constructor
AudioBook::AudioBook(int ID, string title, string author, string category,
                     int totalCopies, string narrator, int duration, string format)
    : LibraryResource(ID, "audiobook", title, author, category, totalCopies)
{
    this->narrator = narrator;
    this->durationMinutes = duration;
    this->format = format;
}

// getters
string AudioBook::getNarrator() const { return narrator; }
int AudioBook::getDuration() const { return durationMinutes; }
string AudioBook::getFormat() const { return format; }

// setters
void AudioBook::setNarrator(string n) { narrator = n; }
void AudioBook::setDuration(int d) { durationMinutes = d; }
void AudioBook::setFormat(string f) { format = f; }

void AudioBook::displayInfo()
{
    cout << "Resource ID  : " << resourceID << endl;
    cout << "Title        : " << title << endl;
    cout << "Author       : " << authorCreator << endl;
    cout << "Category     : " << category << endl;
    cout << "Narrator     : " << narrator << endl;
    cout << "Duration     : " << durationMinutes << " mins" << endl;
    cout << "Format       : " << format << endl;
    cout << "Total Copies : " << totalCopies << endl;
    cout << "Available    : " << availableCopies << endl;
    cout << "Status       : " << availabilityStatus << endl;
}

// ===================== Magazine =====================

// default constructor
Magazine::Magazine() : LibraryResource()
{
    type = "magazine";
    volumeNumber = 1;
    issueNumber = 1;
    publisher = "Unknown";
    publicationDate = "Unknown";
}

// parameterized constructor
Magazine::Magazine(int ID, string title, string publisher, string category,
                   int totalCopies, int volume, int issue, string pubDate)
    : LibraryResource(ID, "magazine", title, publisher, category, totalCopies)
{
    this->volumeNumber = volume;
    this->issueNumber = issue;
    this->publisher = publisher;
    this->publicationDate = pubDate;
}

// getters
int Magazine::getVolumeNumber() const { return volumeNumber; }
int Magazine::getIssueNumber() const { return issueNumber; }
string Magazine::getPublisher() const { return publisher; }
string Magazine::getPublicationDate() const { return publicationDate; }

// setters
void Magazine::setVolumeNumber(int v) { volumeNumber = v; }
void Magazine::setIssueNumber(int i) { issueNumber = i; }
void Magazine::setPublisher(string p) { publisher = p; }
void Magazine::setPublicationDate(string d) { publicationDate = d; }

void Magazine::displayInfo()
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

// ===================== Newspaper =====================

// default constructor
Newspaper::Newspaper() : LibraryResource()
{
    type = "newspaper";
    editionDate = "Unknown";
    region = "Unknown";
    publisher = "Unknown";
}

// parameterized constructor
Newspaper::Newspaper(int ID, string title, string publisher, string category,
                     int totalCopies, string editionDate, string region)
    : LibraryResource(ID, "newspaper", title, publisher, category, totalCopies)
{
    this->editionDate = editionDate;
    this->region = region;
    this->publisher = publisher;
}

// getters
string Newspaper::getEditionDate() const { return editionDate; }
string Newspaper::getRegion() const { return region; }
string Newspaper::getPublisher() const { return publisher; }

// setters
void Newspaper::setEditionDate(string d) { editionDate = d; }
void Newspaper::setRegion(string r) { region = r; }
void Newspaper::setPublisher(string p) { publisher = p; }

void Newspaper::displayInfo()
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