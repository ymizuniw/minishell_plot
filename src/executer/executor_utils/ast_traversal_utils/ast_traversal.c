#include "../../../../includes/minishell.h"

int	ast_traversal(t_ast *node, char **env)
{
	t_ast	*cur;
	int		last_exit_status;
	pid_t	pid;

	if (cur==NULL)
		return (0);
	cur = node;
	last_exit_status = 0;
	if (cur->type==NODE_AND)
	{
		last_exit_status = ast_traversal(node->left, env);
		if (last_exit_status==0)
			last_exit_status = ast_traversal(node->right, env);
	}
	else if (cur->type==NODE_OR)
	{
		last_exit_status = ast_traversal(node->left, env);
		if (last_exit_status!=0)
			last_exit_status = ast_traversal(node->right, env);
	}
	else if (cur->type==NODE_PIPE)
		last_exit_status = exec_pipe(node, env);//if cmd | (cmd), fork() is done three times?
	else if (cur->type==NODE_SUBSHELL)
		last_exit_status = exec_subshell(node, env);
	else
		last_exit_status = exec_command(node, env);
	return (last_exit_status);
}
