#include "../../../../includes/minishell.h"

//conduct redirection and execute command.
int exec_command(t_ast *node, char **env)
{
	int redir_ret = do_redirection(node);
	if (redir_ret<0)
	{
		perror(node->cmd->redir->filename);//redirection failure.
		exit(redir_ret);
	}
	char *path = validate_path(node->cmd->argv[0], env);
	if (path==NULL)
		exit(127);//command not found exit.
	int execve_ret = execve(path, node->cmd->argv, env);
	if (execve_ret<0)
		exit(1);
}
