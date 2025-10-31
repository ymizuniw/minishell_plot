#include "../../includes/minishell.h"

// syntax_check is implemented in syntax_checker.c

void	fgen_tree(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*op_node;

	if (!parent || !cur_token || !*cur_token
		|| token_is_newline_or_eof((*cur_token)->type)
		|| (*cur_token)->type == TK_HEAD)
		return ;
	node = NULL;
	if (token_is_command((*cur_token)->type))
		node = gen_command_node(*parent, cur_token);
	else if (token_is_subshell_close((*cur_token)->type))
		node = gen_subshell_node(*parent, cur_token);
	else
		node = gen_eof_newline_node(*parent, cur_token);
	if (!node)
		return ;
	if (*cur_token && token_is_operator((*cur_token)->type))
	{
		op_node = swap_with_parent(&node, cur_token);
		if (!op_node)
		{
			*parent = node;
			return ;
		}
		fgen_tree(&op_node->right, cur_token);
		if (op_node->right)
			op_node->right->parent = op_node;
		*parent = op_node;
	}
	else
		*parent = node;
}

t_ast	*parse(t_token *token_head)
{
	t_ast	*root;
	t_token	*cur;

	root = NULL;
	if (!token_head || token_head->type != TK_HEAD)
		return (NULL);
	cur = token_head->next;
	if (!cur || cur->type == TK_EOF)
		return (NULL);
	fgen_tree(&root, &cur);
	if (cur && cur->type != TK_EOF && cur->type != TK_HEAD)
		return (NULL);
	return (root);
}

// legacy entry point expected by main.c and headers
t_ast	*parser(t_token *token_list)
{
	return (parse(token_list));
}
