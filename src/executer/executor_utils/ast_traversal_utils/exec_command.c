#include "../../../../includes/minishell.h"

// conduct redirection and execute command.
int	exec_command(t_ast *node, t_env *env_list, int execute, int last_exit_status)
{
	int		redir_ret;
	(void)env_list;
	int		execve_ret;

	if (!node || !node->cmd)
		return (1);
	redir_ret = do_redirection(node);
	if (redir_ret < 0)
	{
		if (node->cmd->redir && node->cmd->redir->filename)
			perror(node->cmd->redir->filename);
		exit(redir_ret);
	}
	if (execute==false)
		return (last_exit_status);
	if (!node->cmd->argv || !node->cmd->argv[0])
		return (0);
	// execve_ret = search_and_exec();
	printf("command executed.\n");
	exit(0);
	if (execve_ret < 0)
		return (1);
}
