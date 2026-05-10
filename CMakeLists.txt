cmake_minimum_required(VERSION 3.10)
project(LibrarySystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output binary to bin/ inside project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# All source files
add_executable(LibrarySystem
    main.cpp
    src/Admin.cpp
    src/FileHandler.cpp
    src/Library.cpp
    src/Menu.cpp
    src/Resources.cpp
    src/title.cpp
    src/Title.cpp
    src/Users.cpp
    src/Utils.cpp
)

# Tell compiler where to find header files
target_include_directories(LibrarySystem PRIVATE include)

# Windows-specific: link against nothing extra, but suppress warnings
if(WIN32)
    target_compile_definitions(LibrarySystem PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
