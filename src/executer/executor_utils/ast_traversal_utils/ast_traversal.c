#include "../../../../includes/minishell.h"

/*
** ast_traversal: Recursively execute an AST tree
**
** IMPORTANT: The parser builds operators with REVERSED left/right structure:
**   - For "a && b", tree is AND(left=b, right=a)
**   - For "a || b", tree is OR(left=b, right=a)
**   - For "a | b",  tree is PIPE(left=b, right=a)
**
** Therefore, we must traverse RIGHT first, then LEFT to maintain correct order.
*/

int	ast_traversal(t_ast *node, t_shell *shell, bool execute)
{
	if (node == NULL || shell == NULL)
		return (0);
	if (node->type == NODE_AND)
	{
		// Execute RIGHT first (due to parser's tree structure)
		// For "a && b", tree is AND(left=b, right=a)
		ast_traversal(node->right, shell, execute);
		if (shell->last_exit_status == 0)
			ast_traversal(node->left, shell, execute);
		return (0);
	}
	else if (node->type == NODE_OR)
	{
		// Execute RIGHT first (due to parser's tree structure)
		// For "a || b", tree is OR(left=b, right=a)
		ast_traversal(node->right, shell, execute);
		if (shell->last_exit_status != 0)
			ast_traversal(node->left, shell, execute);
		return (0);
	}
	else if (node->type == NODE_PIPE)
		exec_pipe(node, shell, execute);
	else if (node->type == NODE_SUBSHELL)
		exec_subshell(node, shell, execute);
	else
		exec_command(node, shell, execute);
	return (0);
}
