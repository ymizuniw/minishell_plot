#ifndef SIGNAL_MANAGE_H
# define SIGNAL_MANAGE_H

# include "minishell_types.h"

int	signal_initializer(bool interactive);
int	set_sig_dfl(void);
int	handle_child(int *last_exit_status, pid_t pid);

#endif
