// builtInFunctions.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "builtInFunctions.h"

extern char **environ;

//Function for built-in command cd (Changes Directories)
void execute_cd(char **arguments) {
    // Check if an argument is provided
    if (arguments[1] == NULL) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }

    // Attempt to change the directory
    if (chdir(arguments[1]) != 0) {
        perror("cd error");
    }
}


//Function for built-in command pwd
void execute_pwd() {
    char cwd[MAX_COMMAND_LINE_LEN];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd error");
    }
}


void execute_echo(char **arguments) {
    int suppress_newline = 0;  // Flag to suppress newline
    if (arguments[1] != NULL && strcmp(arguments[1], "-n") == 0) {
        suppress_newline = 1;  // Set flag to suppress newline
        arguments++;           // Move to the next argument
    }

    // Print the remaining arguments
    for (int i = 1; arguments[i] != NULL; i++) {
        printf("%s", arguments[i]);  // Print argument
        if (arguments[i + 1] != NULL) {
            printf(" ");  // Print space between arguments
        }
    }

    // Print a newline unless suppressed
    if (!suppress_newline) {
        printf("\n");
    }
}

// Function for built-in command exit (Terminates the shell)
void execute_exit() {
    printf("Exiting shell...\n");
    exit(0);
}

// Function for built-in command env (Prints all environment variables)
void execute_env() {
    //extern char **environ;
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }
}

// Function for built-in command setenv (Sets an environment variable)
void execute_setenv(char **arguments) {
    // Check if both variable name and value are provided
    if (arguments[1] == NULL || arguments[2] == NULL) {
        fprintf(stderr, "setenv: missing arguments\nUsage: setenv <variable> <value>\n");
        return;
    }

    // Attempt to set the environment variable
    if (setenv(arguments[1], arguments[2], 1) != 0) {
        perror("setenv error");
    }
}