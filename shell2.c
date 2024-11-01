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
volatile sig_atomic_t in_foreground_process = 0;

//Signal handler for 10 second alarm
void sigalrm_handler (int sig) {
    printf("\nTime limit exceeded! Terminating the function.\n");
    // Kill the process
    kill(getpid(), SIGKILL);
}


// Signal handler for SIGINT
void sigint_handler(int sig) {
    // Write a newline to clean up the shell prompt
    write(STDOUT_FILENO, "\n", 1);
    
    if (!in_foreground_process) {
        // If we're in the shell (no foreground process)
        // Redisplay the prompt
        char cwd[MAX_COMMAND_LINE_LEN];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            write(STDOUT_FILENO, cwd, strlen(cwd));
            write(STDOUT_FILENO, "> ", 2);
        }
    }
}


// Function to check and remove the & symbol from arguments
bool process_background_execution(char **arguments) {
    int i;
    bool is_background = false;
    
    // Find the last argument
    for (i = 0; arguments[i] != NULL; i++);
    
    // Check if last argument is &
    if (i > 0 && strcmp(arguments[i-1], "&") == 0) {
        is_background = true;
        arguments[i-1] = NULL;  // Remove the & from arguments
    }
    
    return is_background;
}

// Function to cleanup zombie processes
void cleanup_background_processes() {
    int status;
    pid_t pid;
    
    // Non-blocking check for any child processes that have finished
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("[%d] Done\n", pid);
        } else if (WIFSIGNALED(status)) {
            printf("[%d] Terminated by signal %d\n", pid, WTERMSIG(status));
        }
    }
}


//Function for parsing input and executing corresponding built in command
void parse_input(char **arguments) {
    bool is_background = process_background_execution(arguments);

    if (arguments[0] == NULL) {
        return;  // No command entered
    }

    // Check if the command is a built-in command
    if (strcmp(arguments[0], "pwd") == 0) {
        execute_pwd();
    } else if (strcmp(arguments[0], "cd") == 0) {
        execute_cd(arguments);
    } else if (strcmp(arguments[0], "echo") == 0) {
        execute_echo(arguments);
    } else if (strcmp(arguments[0], "exit") == 0) {
        execute_exit(arguments);
    } else if (strcmp(arguments[0], "setenv") == 0) {
        execute_setenv(arguments);
    } else if (strcmp(arguments[0], "env") == 0) {
        execute_env();
    } else {
        // Handle external commands
        pid_t pid = fork();
        
        if (pid < 0) {
            // Fork failed
            perror("fork failed");
            return;
        } else if (pid == 0) {
            // Child process

            // Reset signal handler to default for child process
            signal(SIGINT, SIG_DFL);

            // Set up the child process to ignore SIGINT if it's a background process
            if (is_background) {
                signal(SIGINT, SIG_IGN);
            }
            execvp(arguments[0], arguments);
            
            // If execvp returns, it means it failed
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            if (is_background) {
                // For background processes, don't wait
                printf("[%d] %s running in background\n", pid, arguments[0]);
            } else{
                 // Set flag for foreground process
                in_foreground_process = 1;

                // For foreground processes, set alarm after 10 seconds and wait for completion
                alarm(10);
                int status;
                waitpid(pid, &status, 0);

                //Reset alarm on foreground process completion
                alarm(0);
                // Reset foreground process flag on completion
                in_foreground_process = 0;
            
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    printf("Command exited with status %d\n", WEXITSTATUS(status));
                }
            }
        }
    }
}

int main() 
{
    //Set up handler for 10 sec foreground proccess alarm
    signal(SIGALRM, sigalrm_handler);
    // Set up signal handler for Ctrl-C
    signal(SIGINT, sigint_handler);

    // Stores the string typed into the command line.
    char command_line[MAX_COMMAND_LINE_LEN];
    char cmd_bak[MAX_COMMAND_LINE_LEN];
  
    // Stores the tokenized command line input.
    char *arguments[MAX_COMMAND_LINE_ARGS];
    	
    while (true) {
        //Clean up any completed background processes
        cleanup_background_processes();
        // Get the current working directory
        char cwd[MAX_COMMAND_LINE_LEN];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
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

            
        }
    // This should never be reached.
    return -1;
}
