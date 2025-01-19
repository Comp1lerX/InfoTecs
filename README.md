# Logger Library and Console Application
## Description
The project consists of a dynamic library for logging messages with different levels of importance and a console multithreaded application to demonstrate its work. The library provides a simple and efficient way to write logs to a text file with the ability to filter messages by importance level and change the importance level during program execution.

### Features
- Dynamic library for writing log messages to a file.
- Logs include:
  - Message text.
  - Logging level.
  - Timestamp.
- It supports three levels of importance: INFO, WARNING, ERROR (values 1, 2, 3, respectively).
- Filters messages that are below the specified importance level.
- Allows you to change the importance level for a record during program execution.
- A thread-safe implementation for running in a multithreaded environment.
- Accepts messages from the user and their importance level via the console.
- Processes data in a separate thread to demonstrate the library's thread safety.
- Waits for new messages after transmitting the current data.
The application is an example of using the library and provides visibility of its operation.

## Requirements

- C++17 or later
- `g++` compiler
- Linux operating system

## File structure

File structure:
- `liblogger.h` and `liblogger.cpp` : Implementation of the logging library. Features include:
  - Initialize the library with the log file name and the default importance level.
  - Logging messages to a file.
  - Changing the default importance level.
- `main.cpp` : Console application implementation:
  - Connects the library.
  - Accepts messages and their level of importance.
  - Passes the data to a separate stream for writing to the log.
- `Makefile`: Contains commands for building a project, copying a library, and changing environment settings.

## Building and Running

### 1. Clone the Repository

```bash
git clone https://github.com/Comp1lerX/InfoTecs.git
cd InfoTecs
```

### 2. Build the project
```bash
make
```
### 3. Update LD_LIBRARY_PATH
```bash
source ~/.bashrc
```
### 4. Run the application
```bash
./app log.txt 1
```
### 5. Clean the project
```bash
make clean
```
