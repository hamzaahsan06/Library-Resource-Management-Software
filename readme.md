# 📚 Library & Resource Management System
### CS-116 Object Oriented Programming — Term Project
**FE Batch 2025 | Spring Semester 2026**

---

## 👥 Team Members

| Name | Roll No | Section |
|------|---------|---------|
| Shahzad Ahmed Siddiqui | CS-127 | SEC C |
| Muhammad Haseeb | CS-120 | SEC C |
| Muhammad Hamza Ahsan | CS-130 | SEC C |
| Muhammad Moosa Iqbal | CS-143 | SEC C |

---

## 📌 Project Overview

A console-based **Library & Resource Management System** built in C++. It supports multiple user roles (Student, Teacher, Staff, Premium Member, Admin) and multiple resource types (Book, DVD, AudioBook, Magazine, Newspaper). All data is stored and loaded from CSV flat-files automatically on startup and exit.

---

## 🖥️ Prerequisites

Before running this project, ensure the following are installed:

- **OS:** Windows only — the project uses `conio.h` for password masking and `system("cls")` for screen clearing
- **Compiler:** MinGW GCC (g++) or any Windows-compatible C++ compiler
- **Standard:** C++11 or later

### Installing MinGW (if not already installed)

1. Download from [https://sourceforge.net/projects/mingw/](https://sourceforge.net/projects/mingw/)
2. During setup, select the `mingw32-gcc-g++` package
3. Add MinGW's `bin` folder to your system PATH (typically `C:\MinGW\bin`)
4. Verify by running in Command Prompt:
   ```
   g++ --version
   ```

---

## 📂 Project Structure

```
LIBRARY-MANAGEMENT-SYSTEM/
│
├── bin/                        # Compiled output
│   └── LibrarySystem.exe
│
├── database/                   # Flat-file storage (CSV)
│   ├── borrowHistory.csv
│   ├── resources.csv
│   └── users.csv
│
├── docs/                       # UML diagrams and documentation
│
├── include/                    # Header files (.h)
│   ├── Admin.h
│   ├── Colors.h
│   ├── FileHandler.h
│   ├── Library.h
│   ├── Menu.h
│   ├── Resources.h
│   ├── Title.h
│   ├── Users.h
│   └── Utils.h
│
├── src/                        # Implementation files (.cpp)
│   ├── Admin.cpp
│   ├── FileHandler.cpp
│   ├── Library.cpp
│   ├── Menu.cpp
│   ├── Resources.cpp
│   ├── Title.cpp
│   ├── Users.cpp
│   └── Utils.cpp
│
├── main.cpp                    # Entry point
└── README.md
```

---

## ▶️ How to Compile and Run

### Step 1 — Open Command Prompt

### Step 2 — Navigate to the project folder
```
cd path\to\LIBRARY-MANAGEMENT-SYSTEM
```

### Step 3 — Compile the project
```
g++ -o bin/LibrarySystem.exe main.cpp src/Admin.cpp src/FileHandler.cpp src/Library.cpp src/Menu.cpp src/Resources.cpp src/Title.cpp src/Users.cpp src/Utils.cpp -I include
```

### Step 4 — Run the executable
```
.\bin\LibrarySystem.exe
```

> **Important:** The `database/` folder must remain in the same directory as the executable. It holds all CSV data files. Do not delete or move it.

> **Exit properly:** Always exit using option `0` from the main menu so that data is saved back to the CSV files correctly.

---

## 🔐 Default Login Credentials

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

## 🧠 OOP Concepts — Where Each Is Demonstrated

**Inheritance**
The `User` base class is extended by `Student`, `Teacher`, `Staff`, `PremiumMember`, and `Admin`. Similarly, `LibraryResource` is the base for `Book`, `DVD`, `AudioBook`, `Magazine`, and `Newspaper`. Both hierarchies are declared in `Users.h` / `Resources.h` and implemented in their respective `.cpp` files.

**Abstract Classes & Pure Virtual Functions**
Both `User` and `LibraryResource` contain pure virtual functions (`getDailyLimit()`, `getFineRate()`, `getBorrowDays()`, `displayInfo()`), making them abstract. No object of these base classes can be created directly.

**Method Overriding (Polymorphism)**
Each derived class provides its own implementation of `displayInfo()`, `getDailyLimit()`, `getFineRate()`, and `getBorrowDays()`. For example, a `Student` gets 7 borrow days and is fined Rs. 10/day, while a `PremiumMember` with Diamond tier gets 30 days and Rs. 5/day fine.

**Encapsulation**
All class data members are `protected` or `private`. Access is strictly through getters and setters defined in each class.

**Association, Aggregation & Composition**
The `Library` class aggregates `User*` and `LibraryResource*` vectors. It is composed with the nested `BorrowRecord` class, which holds references to both a user ID and a `LibraryResource*`. This is found in `Library.h` and `Library.cpp`.

**Exception Handling**
`try/catch` blocks wrap all major operations in `main.cpp`, `Admin.cpp`, `Library.cpp`, `Utils.cpp`, and `Menu.cpp`. Invalid inputs (wrong type, empty strings, digits-only text) throw `invalid_argument` or `runtime_error` exceptions, which are caught and displayed without crashing the program.

**File Handling (Flat-file Persistence)**
All CSV reading and writing is handled in `FileHandler.cpp`. On startup, `main.cpp` calls the read functions to populate the in-memory vectors. On clean exit, write functions save all changes back. Borrow history, users, and resources each have their own CSV with structured column layouts.

**Soft Delete**
Rather than erasing users or resources from memory, an `isDeleted` flag marks them as inactive. This preserves historical borrow records that still reference them.

**Membership Upgrade Logic**
`Library::checkAndUpgradeUser()` in `Library.cpp` detects when a user's balance reaches Rs. 500 or more after a deposit and automatically replaces their object with a new `PremiumMember` pointer, freeing the old one.

---

## ✅ What Works

- Role-based login with masked password entry
- Full borrow and return flow with automatic fine calculation
- Daily borrow limits enforced per user type
- Resource search by ID, title, author, category, or type
- Admin features: add/update/delete resources and users, view reports, export to file, fine management, donation collection
- User features: view profile, borrow history, deposit funds, change password
- Automatic membership upgrade when balance threshold is met
- All data saved and restored between sessions via CSV files
- Colored CLI output for improved readability
- Guest view of available resources (no login required)

## ⚠️ What Doesn't Work / Known Limitations

- The project is **Windows-only** due to `conio.h` usage. It will not compile or run on Linux or macOS without modifications.
- There is no input validation on username uniqueness during registration — two users can be registered with the same username.
- The `generateNewUserID()` and `generateNewResourceID()` functions read directly from the CSV file path `../database/...`, which may fail depending on where the executable is run from.

---

## 📖 Resources Used

1. **YouTube** — for understanding OOP concepts, CSV file handling in C++, and ANSI color codes
2. **AI Models** — especially Claude, for assistance with debugging, code structure, and writing logic for certain features

---

## 👨‍💻 Individual Contributions

All team members actively participated across the full development lifecycle — including system design, implementation, testing, and documentation — with primary responsibilities distributed as follows:

**Shahzad** led the implementation of exception handling across the entire codebase, ensuring system robustness and fault tolerance. He was the primary developer for the admin module, delivering key functionalities such as resource management, user management, and reporting features. He also contributed to the architectural design of the UML class diagram and participated in structuring the main application entry point.

**Haseeb** implemented several core Library class functions and cross-checked the desired output by manually executing and verifying each feature. He also contributed to a few admin functions and helped ensure correctness of the overall system behavior. He also worked on adding eye-catching colours to the project.

**Hamza** was responsible for the foundational architecture of the project. His contributions include designing and implementing the database structure and CSV-based file handling system, organizing the codebase into a modular folder structure with clearly separated header and source files, and authoring the UML class diagram. He performed systematic debugging across the project, conducted regular code reviews of teammates' work throughout the development cycle, and implemented the complete User and Resource class hierarchies — including all associated header and source files. He also developed the application's title and startup screen, as well as the user membership upgrade feature.

**Moosa** contributed to the implementation of Library class functions and assisted in developing the User and Resource class hierarchies. He further assisted in the CLI's visual presentation by integrating a color-coded interface. In addition, he configured CMake build support for the project and took the lead in preparing the project documentation and final report.
