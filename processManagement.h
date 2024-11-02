#ifndef PROCESS_MANAGEMENT_H
#define PROCESS_MANAGEMENT_H

#include <stdbool.h>

bool process_background_execution(char **arguments);
void cleanup_background_processes();
void execute_command(char **arguments);

#endif
