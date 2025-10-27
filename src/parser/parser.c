#include "../../includes/minishell.h"

t_ast	*parser(t_token **token_head)
{
	t_token	*token;
	t_ast	*root;

	if (!token_head)
		return (NULL);
	root = NULL;
	token = (*token_head)->next;
	if (!token)
		return (NULL);
	root = gen_tree(NULL, &token, 0);
	if (!root)
	{
		assert(root==NULL);
		return (NULL);
	}
	while (token && token->type == TK_NEWLINE)
		token = token->next;
	*token_head = token;
	return (root);
}
