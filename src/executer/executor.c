#include "../../includes/minishell.h"

t_result *executor(t_ast *ast, t_shell *shell)
{
	t_result *res = alloc_result();
	ast_traversal(ast, shell->env_list, true, 0);
	res->root = ast;
	res->exit_code = shell->last_exit_status;
	return (res);
}
