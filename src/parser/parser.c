#include "../includes/minishell.h"

t_ast	*parser(t_token *token_head)
{
	t_token	*token;
	t_ast *root=NULL;

	token = token_head->next;
	root = gen_tree(root,token, 0);
	return (root);
}
