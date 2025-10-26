#include "../../includes/minishell.h"

t_ast	*parser(t_token *token_head)
{
	t_token	*token;
	t_ast	*root;

	if (!token_head)
		return (NULL);
	root = NULL;
	// move to the first meaningful token
	token = token_head->next;
	while (token && (token->type == TK_HEAD || token->type == TK_NEWLINE))
		token = token->next;
	if (!token || token->type == TK_EOF)
		return (NULL);
	root = gen_tree(NULL, &token, 0);
	// ensure we consumed trailing newlines; anything else is syntax error
	while (token && token->type == TK_NEWLINE)
		token = token->next;
	if (token && token->type != TK_EOF)
	{
		syntax_error(token->type);
		free_ast_tree(root);
		return (NULL);
	}
	return (root);
}
