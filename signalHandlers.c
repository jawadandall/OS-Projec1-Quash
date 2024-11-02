#include "signalHandlers.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

extern volatile sig_atomic_t in_foreground_process;

void sigalrm_handler(int sig) {
    printf("\nTime limit exceeded! Terminating the process.\n");
    kill(getpid(), SIGKILL);
}

void sigint_handler(int sig) {
    write(STDOUT_FILENO, "\n", 1);
    if (!in_foreground_process) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            write(STDOUT_FILENO, cwd, strlen(cwd));
            write(STDOUT_FILENO, "> ", 2);
        }
    }
}

void setup_signal_handlers() {
    signal(SIGALRM, sigalrm_handler);
    signal(SIGINT, sigint_handler);
}