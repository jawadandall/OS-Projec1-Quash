# Shell Implementation Project Report

## Project Overview
This comprehensive shell project aims to create a custom command-line interface that mimics core functionality of UNIX shells like bash. The assignment is structured as a progressive implementation with six distinct tasks, each adding complexity and sophistication to the shell's capabilities.

## Project Requirements Breakdown

### Task 1: Basic Shell Functionality
The first task establishes the foundational shell features:

#### Prompt Display
- Use `getcwd()` to dynamically display current working directory
- Format prompt as `<path>> ` (e.g., `/home/codio>`)

#### Built-in Commands Implementation
1. `cd`: Change current working directory
2. `pwd`: Print current working directory
3. `echo`: Print messages and environment variables
4. `exit`: Terminate shell
5. `env`: Display environment variables
6. `setenv`: Set environment variables

#### Key Technical Challenges
- Parsing command-line input
- Handling environment variable expansion (e.g., `$HOME`)
- Implementing built-in command logic

### Task 2: Process Management
Extend shell to execute external commands:
- Detect non-built-in commands
- Use `fork()` to create child processes
- Execute commands using `execvp()`
- Wait for child process completion
- Error reporting for failed executions

### Task 3: Background Process Support
- Detect `&` at command end
- Allow non-blocking command execution
- Immediate shell prompt return for background processes

### Task 4: Signal Handling
- Implement `SIGINT` (Ctrl+C) handler
- Prevent shell termination during foreground process execution
- Return to prompt after signal interruption

### Task 5: Process Time Limitation
- Implement 10-second timeout for foreground processes
- Use `kill()` to terminate long-running processes
- Restore shell prompt after timeout

### Task 6 (Optional/Extra Credit): I/O Redirection
Options include:
- Input redirection (`<`)
- Output redirection (`>`)
- Piping between processes (`|`)

## Implementation Strategy

### Modular Design
The implementation leverages a modular approach with separate source files:

1. **shell2.c**: Main shell logic and event loop
2. **commandParser.c**: Command parsing and routing
3. **builtInFunctions.c**: Built-in command implementations
4. **processManagement.c**: Process creation and management
5. **signalHandlers.c**: Advanced signal handling

### Technical Design Principles
- Separation of concerns
- Comprehensive error handling
- Flexible command parsing
- Extensible architecture

## Key Technical Implementations

### Command Parsing
- Tokenize input using `strtok()`
- Support variable argument count
- Handle environment variable expansion

### Process Management
- Use `fork()` for process creation
- `execvp()` for command execution
- `waitpid()` for process tracking
- Background process detection

### Signal Handling
- `sigaction()` for robust signal management
- Differentiate foreground/background process signals
- Prevent shell interruption

### Environment Variable Management
- Use global `environ` pointer
- Implement dynamic variable setting and retrieval

## Compilation and Execution
```bash
gcc -o shell shell2.c commandParser.c builtInFunctions.c processManagement.c signalHandlers.c
```

## Learning Outcomes
This project provides hands-on experience with:
- UNIX system programming
- Process management
- Signal handling
- Command-line interface design
- C programming techniques

## Conclusion
The shell implementation demonstrates fundamental operating system concepts through a practical, incrementally developed command-line interface.
