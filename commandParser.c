#include "commandParser.h"
#include "builtInFunctions.h"
#include "processManagement.h"
#include <string.h>

void parse_input(char **arguments) {
    if (arguments[0] == NULL) {
        return;
    }

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
        execute_command(arguments);
    }
}
