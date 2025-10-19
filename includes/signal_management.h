#ifndef SIGNAL_MANAGE_H
#define SIGNAL_MANAGE_H

#include <signal.h>

void *sig_int_event(void);
void *sig_term_event(void);
void signal_initializer(int *g_set);
void *signal_handler(int signum);

#endif