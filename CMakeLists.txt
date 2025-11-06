# Minimum CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project name
project(ContactManager CXX)

# Set the C++ standard to C++11 (or higher, e.g., 17)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define source files for the executable
set(SOURCE_FILES
    main.cpp
    BST.cpp
    FileIO.cpp
    LinkedListHistory.cpp
    ShareHistory.cpp
)

# Add the executable target
add_executable(contact_manager ${SOURCE_FILES})

# Optional: Set compilation flags for warnings and debugging
target_compile_options(contact_manager PRIVATE -Wall -Wextra -g)