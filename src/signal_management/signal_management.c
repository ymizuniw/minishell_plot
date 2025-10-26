#include "../../includes/minishell.h"

volatile sig_atomic_t g_signum = 0;

static void signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_signum = SIGINT;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();		
	}
}

//just before execve, rest it default sigaction.
int set_sig_dfl(void)
{
	struct sigaction sact;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
	sact.sa_handler = SIG_DFL;
	sigaction(SIGTERM, &sact NULL);
	sigaction(SIGQUIT, &sact NULL);
	sigaction(SIGINT, &sact NULL);
	return (0);
}

int	signal_initializer(bool interactive)
{
	struct sigaction sact;
	
	if (interactive==true)
	{
		sigemptyset(&sact.sa_mask);
		sact.sa_flags = 0;
		sact.sa_handler = SIG_IGN;
		sigaction(SIGTERM, &sact, NULL);
		sigaction(SIGQUIT, &sact, NULL);

		sact.sa_flags = 0;
		sact.sa_handler = signal_handler;
		sigaction(SIGINT, &sact, NULL);
		return (0);
	}
	set_sig_dfl();
	return(0);
}

//when bash is in interactive mode, SIGTERM are ignored. SIGINT is handled, and SIGQUIT is ignored in all cases.
//if asyncronous command is under execution, SIGTERM and SIGINT are also ignored.
int handle_child(int *last_exit_status, pid_t pid)
{
	int local_status = 0;
	if (waitpid(pid, &local_status, 0)==-1)
	{
		perror("waitpid");
		*last_exit_status = 1;
		return -1;
	}
	if (WIFSIGNALED(local_status))
	{
		*last_exit_status = WEXITSTATUS(local_status);
		if (WTERMSIG(local_status)==SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(local_status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	return 0;
}

//Reference for child signal exit value.
// void		status_child(void)
// {
// 	if (WIFEXITED(g_pid))
// 		g_status = WEXITSTATUS(g_pid);
// 	if (WIFSIGNALED(g_pid))
// 	{
// 		g_status = WTERMSIG(g_pid);
// 		if (g_status != 131)
// 			g_status += 128;
// 	}
// }
