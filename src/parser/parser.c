#include "../../includes/minishell.h"

t_ast	*parser(t_token *token_head)
{
	t_token	*token;
	t_ast	*root;

	//if there is no token, then return NULL;
	if (!token_head)
		return (NULL);
	//init root.
	root = NULL;
	//first token is dummy head, so the real head is token_head->next.
	token = token_head->next;
	//gen_tree() returns the ptr to the root of the tree. 
	root = gen_tree(NULL, &token, 0, 0);
	return (root);
}
