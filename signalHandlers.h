#ifndef SIGNAL_HANDLERS_H
#define SIGNAL_HANDLERS_H

#include <signal.h>

void sigalrm_handler(int sig);
void sigint_handler(int sig);
void setup_signal_handlers();

#endif
