#include "../includes/minishell.h"

void	*sig_int_event(void)
{
    int clear_undo=1;
    char *text="";

	text = "";
	rl_on_new_line();
	rl_replace_line(text, clear_undo);
	rl_redisplay();
}

void	*sig_term_event(void)
{
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	rl_clear_history();
}

void	*signal_handler(int signum)
{
	if (signum == SIGINT)
		signal(signum, sig_int_event);
	else if (signum == SIGTERM)
		signal(signum, sig_term_event);
}

void	signal_initializer(int *g_set)
{
	sigemptyset(g_set);
	sigaddset(g_set, SIGINT);
}
