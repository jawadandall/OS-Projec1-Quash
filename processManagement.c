#include "processManagement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

extern volatile sig_atomic_t in_foreground_process;

bool process_background_execution(char **arguments) {
    int i;
    bool is_background = false;
    for (i = 0; arguments[i] != NULL; i++);
    if (i > 0 && strcmp(arguments[i-1], "&") == 0) {
        is_background = true;
        arguments[i-1] = NULL;
    }
    return is_background;
}

void cleanup_background_processes() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("[%d] Done\n", pid);
        } else if (WIFSIGNALED(status)) {
            printf("[%d] Terminated by signal %d\n", pid, WTERMSIG(status));
        }
    }
}

void execute_command(char **arguments) {
    bool is_background = process_background_execution(arguments);
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        return;
    } else if (pid == 0) {
        signal(SIGINT, is_background ? SIG_IGN : SIG_DFL);
        execvp(arguments[0], arguments);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        if (is_background) {
            printf("[%d] %s running in background\n", pid, arguments[0]);
        } else {
            in_foreground_process = 1;
            alarm(10);
            int status;
            waitpid(pid, &status, 0);
            alarm(0);
            in_foreground_process = 0;
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                printf("Command exited with status %d\n", WEXITSTATUS(status));
            }
        }
    }
}
