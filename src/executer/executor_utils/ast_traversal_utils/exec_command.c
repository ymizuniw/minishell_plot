#include "../../../../includes/minishell.h"

// conduct redirection and execute command.
int	exec_command(t_ast *node, t_shell *shell)
{
	int	redir_ret;

	(void)shell;
	if (!node || !node->cmd)
		return (-1);
	redir_ret = do_redirection(node);
	if (redir_ret < 0)
	{
		if (node->cmd->redir && node->cmd->redir->filename)
			perror(node->cmd->redir->filename);
		return (1);
	}
	if (!node->cmd->argv || !node->cmd->argv[0])
		return (0);
	return (0);
}
