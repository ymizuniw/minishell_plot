#include "../../includes/minishell.h"

t_result *executor(t_ast *ast, char **env)
{
	t_result *res = alloc_result();
	int last_exit_status = ast_traversal(ast, env, true, 0);
	res->root = ast;
	res->exit_code = last_exit_status;
	return (res);
}
