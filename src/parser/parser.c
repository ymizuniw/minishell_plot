#include "../../includes/minishell.h"

t_ast	*parser(t_token **token_head)
{
	t_token	*token;
	t_ast	*root;

	if (!token_head || !*token_head)
		return (NULL);
	root = NULL;
	token = (*token_head)->next;
	if (!token || token->type == TK_EOF)
		return (NULL);
	root = gen_tree(NULL, &token, 0);
	if (!root)
	{
		while (token && token->type != TK_NEWLINE && token->type != TK_EOF)
			token = token->next;
		return (NULL);
	}
	while (token && token->type == TK_NEWLINE)
		token = token->next;
	if (token && token->type != TK_EOF)
	{
		syntax_error(token->type);
		if (root)
			free_ast_tree(root);
		return (NULL);
	}
	return (root);
}
