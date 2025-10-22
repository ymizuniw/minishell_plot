#include "../../includes/minishell.h"

t_result *executor(t_ast *node, char *env)
{
	int status = 0;
	status = ast_traversal(node, env);
	t_result *res = alloc_result();
	res->exit_code=status;
	res->root=node;
	return (res);
}