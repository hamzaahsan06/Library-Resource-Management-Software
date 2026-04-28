# Library and Resource Management System

## CS-116 Object Oriented Programming — Term Project
**FE Batch 2025, Spring Semester 2026**

---

## Team Members

| Name    | Roll No |
|---------|---------|
| Hamza   | CS-130  |
| Shahzad | CS-127  |
| Haseeb  | CS-120  |
| Moosa   | CS-143  |

---

## Project Overview

This is a console-based Library and Resource Management System developed in C++. The system manages library resources and member activities by tracking resource availability, borrowing history, and fine calculation. It supports multiple user types (Student, Teacher, Staff, Premium Member, Admin) and multiple resource types (Book, DVD, AudioBook, Magazine, Newspaper).

---

## Prerequisites

Before running this project, make sure the following are installed on your PC:

- **Operating System:** Windows (required — the project uses `conio.h` for password masking and `system("cls")` for screen clearing)
- **Compiler:** MinGW GCC (g++) or any Windows-compatible C++ compiler
- **C++ Standard:** C++11 or later

### Installing MinGW (if not already installed)

1. Download MinGW from [https://sourceforge.net/projects/mingw/](https://sourceforge.net/projects/mingw/)
2. During installation, select `mingw32-gcc-g++` package
3. Add MinGW `bin` folder to your system PATH (usually `C:\MinGW\bin`)
4. Verify installation by opening Command Prompt and running:

```
g++ --version
```

---

## Project Structure

```
Project/
│
├── main.cpp
│
├── Include/
│   ├── Library.h
│   ├── library.cpp
│   ├── Resources.h
│   ├── Resources.cpp
│   ├── Users.h
│   ├── Users.cpp
│   ├── admin.h
│   ├── admin.cpp
│   ├── utils.h
│   └── utils.cpp
│
├── FileHandling/
│   ├── FileHandler.h
│   └── FileHandler.cpp
│
└── database/
    ├── users.csv
    ├── resources.csv
    └── borrowHistory.csv
```

---

## How to Compile and Run

### Option 1: Using Command Prompt (Recommended)

1. Open Command Prompt
2. Navigate to the root project folder:

```
cd path\to\project
```

3. Run the following compile command:

```
g++ main.cpp Include/utils.cpp Include/admin.cpp Include/library.cpp  Include/Users.cpp Include/Resources.cpp FileHandling/FileHandler.cpp -Iinclude -Ifilehandling -o app

```

4. Run the executable:

```
.\app.exe
```

### Option 2: Using an IDE (Code::Blocks / Dev-C++)

1. Open your IDE
2. Create a new project and add all `.cpp` files listed above
3. Make sure the `database` folder is placed in the same directory as the executable after build
4. Build and run the project

---

## Important Notes for Setup

- The `database` folder **must be present** in the same directory where you run the executable. It contains the CSV files that store all user, resource, and borrow history data. Do not delete or move this folder.
- The project **only runs on Windows** due to `conio.h` dependency used for password masking during login.
- All data is automatically saved back to CSV files when you exit the program cleanly using option `0` from the main menu. Do not force close the program.
- The database folder already contains pre-populated data with 640+ users and 800 resources for testing purposes.

---

## Default Login Credentials

### Admin Accounts

| Username | Password |
|----------|----------|
| admin1 | admin1@123 |
| admin2 | admin2@123 |
| admin3 | admin3@123 |

### Sample User Accounts (for testing)

| Username | Password | Type |
|----------|----------|------|
| bilal1 | bilal1@123 | Student |
| imran501 | imran501@123 | Teacher |
| ali601 | ali601@123 | Staff |
| salman701 | salman701@123 | Premium Member |

---

## Features

- Role-based login system (Student, Teacher, Staff, Premium Member, Admin)
- Multiple resource types: Book, DVD, AudioBook, Magazine, Newspaper
- Borrow and return system with automatic fine calculation for overdue items
- Role-specific borrow limits and borrow durations
- Resource search by ID, Title, Author, Category, or Type
- Admin reports for issued resources, overdue resources, and customer history
- Persistent data storage through CSV file handling
- Password masking during login
- Colored CLI interface for better readability
- Exception handling for invalid inputs throughout the system

---

## Individual Contributions

**Shahzad** implemented exception handling throughout the project and developed the majority of admin-related functionalities including resource management, user management, and reporting features. He also assisted in designing the UML class diagram and contributed to the main function structure.

**Haseeb** implemented several core Library class functions and cross-checked the desired output by manually executing and verifying each feature. He also contributed to one or two admin functions and helped ensure correctness of the overall system behavior.

**Hamza** designed and built the database structure and CSV-based file handling system, organized the entire codebase into a proper folder structure with separate header and source files, designed the UML class diagram, debugged errors across the project, reviewed and examined teammates' code throughout development, and implemented the User and Resource class hierarchies including all header and source files.

**Moosa** contributed several Library class functions, assisted in developing the User and Resource class hierarchies, and added a colored interface to the CLI for improved visual presentation.

---

## OOP Features Implemented

1. **Inheritance** — User base class with Student, Teacher, Staff, PremiumMember, Admin as derived classes. LibraryResource base class with Book, DVD, AudioBook, Magazine, Newspaper as derived classes.
2. **Abstract Classes** — User and LibraryResource both contain pure virtual functions.
3. **Method Overriding** — displayInfo(), getDailyLimit(), getFineRate(), getBorrowDays() are overridden in each derived class.
4. **Association / Aggregation / Composition** — Library aggregates Users and LibraryResources, and is composed with BorrowRecord. BorrowRecord references both User and LibraryResource.
5. **Exception Handling** — try/catch blocks used throughout main.cpp and admin.cpp for robust error handling.
