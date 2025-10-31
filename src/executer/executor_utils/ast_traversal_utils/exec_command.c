#include "../../../../includes/minishell.h"

// conduct redirection and execute command.
int	exec_command(t_ast *node, t_shell *shell)
{
	int		redir_ret;
	char	**expanded_argv;

	if (!node || !node->cmd)
		return (-1);
	redir_ret = do_redirection(node);
	if (redir_ret < 0)
	{
		if (node->cmd->redir && node->cmd->redir->filename)
			perror(node->cmd->redir->filename);
		return (1);
	}
	if (!node->cmd->argv_list)
		return (0);
	expanded_argv = gen_argv(node->cmd->argv_list, shell);
	if (!expanded_argv || !expanded_argv[0])
	{
		xfree(expanded_argv);
		return (0);
	}
	search_and_exec(shell, expanded_argv);
	for (int i = 0; expanded_argv[i]; i++)
		xfree(expanded_argv[i]);
	xfree(expanded_argv);
	return (0);
}
