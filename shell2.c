#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "builtInFunctions.h"  // Include the header file for built in functions


#define MAX_COMMAND_LINE_LEN 1024
#define MAX_COMMAND_LINE_ARGS 128

char prompt[1024];
char delimiters[] = " \t\r\n";
extern char **environ;


//Function for parsing input and executing corresponding built in command
void parse_input(char **arguments) {
    if (arguments[0] == NULL) {
        return;  // No command entered
    }

    // Check if the command is a built-in command
    if (strcmp(arguments[0], "pwd") == 0) { //check if pwd
        execute_pwd();
    } else if (strcmp(arguments[0], "cd") == 0) { //check if cd
        execute_cd(arguments);
    } else if (strcmp(arguments[0], "echo") == 0) { //check if echo
        execute_echo(arguments);
    }else if (strcmp(arguments[0], "exit") == 0) { //check if exit
        execute_exit(arguments);
    } else if (strcmp(arguments[0], "setenv") == 0) { //check if setenv
        execute_setenv(arguments);
    } else if (strcmp(arguments[0], "env") == 0) {  //check if env
        execute_env();
    } else if (strcmp(arguments[0], "ls") == 0) {  //check if ls
      execute_ls(arguments);
    } else { //If command unrecognized
        printf("Unknown command: %s\n", arguments[0]); 
    }
}

int main() 
{
    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    	
    while (true) {
      // Get the current working directory
      char cwd[MAX_COMMAND_LINE_LEN];
      if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Find the last slash in the current working directory
        //char *lastSlash = strrchr(cwd, '/');
        
        // if (lastSlash != NULL && *(lastSlash + 1) != '\0') {
        //     // Format the prompt as "<path>/<cur dir> >"
        //     snprintf(prompt, sizeof(prompt), "%s> ", cwd);
        // } else {
          snprintf(prompt, sizeof(prompt), "%s> ", cwd);
      } else {
          perror("getcwd error");
          exit(EXIT_FAILURE);
        }
    
          do { 
            // Print the shell prompt.
            printf("%s", prompt);
            fflush(stdout);

            
            // Read input from stdin and store it in command_line. If there's an
            // error, exit immediately.        
            if ((fgets(command_line, MAX_COMMAND_LINE_LEN, stdin) == NULL) && ferror(stdin)) {
                fprintf(stderr, "fgets error");
                exit(0);
            }
 
        } while(command_line[0] == 0x0A);  // while just ENTER pressed
        command_line[strlen(command_line) - 1] = '\0';
      
        // If the user input was EOF (ctrl+d), exit the shell.
        if (feof(stdin)) {
            printf("\n");
            fflush(stdout);
            fflush(stderr);
            return 0;
        }

        // Remove newline character from input
        command_line[strcspn(command_line, "\n")] = 0;
			
        // 1. Tokenize the command line input (split it on whitespace)
        int arg_count = 0;
        char *token = strtok(command_line, delimiters);
        while (token != NULL && arg_count < MAX_COMMAND_LINE_ARGS - 1) {
            arguments[arg_count++] = token;
            token = strtok(NULL, delimiters);
        }
        arguments[arg_count] = NULL;  // Null-terminate the array of arguments

        //Parse user input to match to built-in function
        parse_input(arguments);

        // For testing purposes, print out each argument
        // for (int i = 0; i < arg_count; i++) {
        //     printf("Argument[%d]: %s\n", i, arguments[i]);
        // }

        //TODO      
        // 2. Implement Built-In Commands
        
      
    
        // 3. Create a child process which will execute the command line input

  
        // 4. The parent process should wait for the child to complete unless its a background process
      
      
        // Hints (put these into Google):
        // man fork
        // man execvp
        // man wait
        // man strtok
        // man environ
        // man signals
        
        // Extra Credit
        // man dup2
        // man open
        // man pipes
    }
    // This should never be reached.
    return -1;
}
