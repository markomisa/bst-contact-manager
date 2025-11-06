# 📚 Modular C++ Contact Management System

### 💻 Overview

This project implements a modular **Contact Management System** in C++. It features three primary, distinct data structures to handle different aspects of contact management:
1.  A **Binary Search Tree (BST)** to store and efficiently search the main contact directory.
2.  A **Linked List** (implemented as a Most Recently Used/MRU cache) to manage communication history.
3.  A **Vector** to manage sharing history, prioritizing frequently used contacts.

The code has been refactored into multiple files for improved **modularity** and **readability**.

***

### 📄 Detailed Project Description

This application provides a comprehensive solution for contact management:

* **Contact Directory (BST):** Contacts are stored in a **Binary Search Tree (BST)**, ordered primarily by **surname** and secondarily by **name**. This structure enables efficient $O(\log N)$ searching, insertion, and deletion.
* **Communication History (MRU Linked List):** Communication events are tracked using a **Most Recently Used (MRU) Linked List** (a limited-size cache). Communicating with a contact moves them to the front (MRU position). If the list is full, the contact at the tail (LRU position) is evicted.
* **Sharing History (Priority Vector):** Tracks content sharing activity using a **`std::vector`**. Contacts are prioritized based on recent activity. Inactivity is tracked by the counter `x`. Contacts shared with are promoted one position up and have their counter reset. Contacts whose inactivity count exceeds the threshold (`h` or `neaktivnost`) are deprioritized by being moved to the back of the list.

***

### 📂 Project Structure

| File | Responsibility |
| :--- | :--- |
| `main.cpp` | Contains the **main application loop** and user interface (UI). |
| `Contact.h` | Defines the core data structures: `Contact`, `Listnode`, `ListHeader`, and `Treenode`. |
| `BST.h`/`.cpp` | Implements the **Binary Search Tree** logic (creation, search, insertion, deletion, and traversal). |
| `LinkedListHistory.h`/`.cpp` | Implements the **Communication History** (MRU Linked List) logic. |
| `ShareHistory.h`/`.cpp` | Implements the **Sharing History** (Vector with inactivity tracking) logic. |
| `FileIO.h`/`.cpp` | Handles **reading contact data** from the `Contacts20.txt` file. |
| `Contacts20.txt` | Sample CSV file used for initial data loading. |
| `CMakeLists.txt` | Configuration file for building the project using CMake. |
| `.gitignore` | Ignores generated build artifacts and executable files. |

***

### ⚙️ Building and Running

This project uses **CMake** to manage the build process.

#### Prerequisites

You must have the following tools installed on your system:
* **A C++ Compiler** (e.g., GCC or Clang)
* **CMake** (version 3.10 or higher)

#### Steps

1.  **Create Dummy Data:** Ensure a file named `Contacts20.txt` exists in the root directory with a header line and contact entries (Surname, Name, Number).
2.  **Create Build Directory:** Create a separate directory for the build output.

    ```bash
    mkdir build
    cd build
    ```

3.  **Configure with CMake:** Run CMake from inside the `build` directory.

    ```bash
    cmake ..
    ```

4.  **Build the Project:** Compile the source code.

    ```bash
    make
    ```

5.  **Run the Application:** Execute the built program.

    ```bash
    ./contact_manager
    ```