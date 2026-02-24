# Unix Shell (C++)

A minimal Unix-like interactive shell implemented in C++ using POSIX system calls.  
This project supports command execution, input/output redirection, and basic pipelines without relying on external libraries.

---

## Overview

This shell provides:

- Interactive command input
- Execution of external programs using `fork()` and `execvp()`
- Input (`<`) and output (`>`) redirection
- Command pipelines using `|`
- Built-in `cd` command
- Basic file descriptor management

The goal of this project is to demonstrate understanding of process creation, system calls, and Unix file descriptor handling.

---

## Features

### Process Execution
- Creates child processes using `fork()`
- Executes programs using `execvp()`
- Parent process waits for child completion

### Input / Output Redirection
- Supports output redirection using `>`
- Supports input redirection using `<`
- Uses `open()` and `dup2()` to redirect file descriptors

### Pipeline Support
- Connects multiple commands using `|`
- Implements inter-process communication using `pipe()`
- Wires file descriptors between adjacent commands

Example:
```bash
ls -l | grep txt > output.txt
```

### Built-in Command
- `cd` implemented using `chdir()`

---

## Example Usage

```bash
myshell> ls
myshell> cat text.txt
myshell> sort < text.txt
myshell> ls | grep cpp
myshell> ls | grep cpp > result.txt
myshell> cd ..
```

---

## Project Structure

- `main.cpp` – Main shell loop and command execution logic
- `shelpers.cpp` – Command parsing and helper utilities
- `shelpers.h` – Data structures and function declarations
- `Makefile` or `CMakeLists.txt` – Build configuration

---

## How It Works

1. The shell reads user input interactively.
2. Input is tokenized and parsed into commands.
3. If the command is `cd`, it is handled internally.
4. For external commands:
   - The shell calls `fork()`
   - In the child process:
     - Sets up redirection or pipe connections using `dup2()`
     - Calls `execvp()` to execute the program
   - The parent process waits for execution to complete

---

## Skills Demonstrated

- Process management using `fork()` and `execvp()`
- Inter-process communication with `pipe()`
- File descriptor manipulation via `dup2()`
- I/O redirection handling
- Basic command parsing and tokenization
- POSIX system-level programming

---

## Tech Stack

- C++
- POSIX system calls
- Unix file descriptor model
