#ifndef SIGNAL_MANAGE_H
#define SIGNAL_MANAGE_H

#include "minishell.h"

void sig_int_event(int signum);
void sig_term_event(int signum);
void signal_initializer(int *g_set);
void signal_handler(int signum);

#endif