// builtInFunctions.h
#ifndef BUILT_IN_FUNCTIONS_H
#define BUILT_IN_FUNCTIONS_H

#define MAX_COMMAND_LINE_LEN 1024

void execute_cd(char **arguments);
void execute_pwd();
void execute_echo(char **arguments);
void execute_exit();
void execute_setenv(char **arguments);
void execute_env();


#endif
