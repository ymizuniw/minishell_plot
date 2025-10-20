#include "../../../includes/minishell.h"

static int is_operator(t_token_type type)
{
	if (type==TK_NEWLINE || type==TK_PIPE || type==TK_AND_IF || type==TK_OR_IF || type == TK_LPAREN || type==TK_RPAREN)
		return (1);
	return (0);
}

//swap current node and its parent node, and set the parent node as current node's right node.
//you should add NULL handler.
static void swap_and_set_right_node(t_ast *node, t_ast *parent)
{
	t_ast *grand_node = parent->parent;
	t_ast *new_right_node = parent;
	if (grand_node->left==parent)
		grand_node->left=node;
	else if (grand_node->right==parent)
		grand_node->right=node;
	parent->parent=node;
	parent->left=NULL;
	parent->right=NULL;
	node->right=parent;
}

//generate a tree of command.
t_ast	*gen_tree(t_ast *ast, t_token *token, int subshell)
{
	t_ast			*node;

	node = alloc_node();
	bzero(node, sizeof(t_ast));
	if (is_operator(token->type))
	{
		if (token->type == TK_AND_IF)
			node->type = NODE_AND;
		else if (token->type == TK_OR_IF)
			node->type = NODE_OR;
		else if (token->type == TK_PIPE)
			node->type = NODE_PIPE;
		swap_and_set_right_node(node, node->parent);
		node->left = gen_tree(node->left, token->next, subshell);
	}
	else if (token->type==TK_RPAREN)
	{
		node->type = NODE_SUBSHELL;
		if (!syntax_check(token))
			return (NULL);
		node->subtree = gen_tree(node->subtree, token, 1);
		if (node->subtree==NULL)
			return (NULL);
	}
	else if (token->type == TK_LPAREN)
	{
		if(subshell == 1)
			return (node);
		else
		{
			syntax_error(TK_LPAREN);
			return (NULL);
		}
	}
	else if (token->type == TK_WORD || token->type==TK_DOLLER)
	{
		size_t i = 0;
		node->type = NODE_CMD;
		while (token->next && (token->type == TK_WORD || token->type == TK_REDIRECT_IN || token->type == TK_REDIRECT_OUT || token->type == TK_HEREDOC || token->type == TK_APPEND || token->type == TK_DOLLER))
			token = token->next;
		t_token *keep_token = token;
		syntax_check(token);
		if (token->type == TK_REDIRECT_IN || token->type == TK_REDIRECT_OUT || token->type == TK_HEREDOC || token->type == TK_APPEND)
			node->cmd->redir_in = parse_redirection(node->cmd->redir_in, token);
		if (token->type == TK_WORD || token->type == TK_DOLLER)
		{
			node->cmd->argv = realloc(node->cmd->argv, sizeof(char *) * (i + 3));
			node->cmd->argv[i] = expand_value(token);
			if (token->prev->type == TK_WORD || token->prev->type == TK_DOLLER)
				set_argv(node->cmd->argv, token->prev, i + 1);
		}
		if (token->type == TK_REDIRECT_IN || token->type == TK_REDIRECT_OUT || token->type == TK_HEREDOC || token->type == TK_APPEND)
			node->cmd->redir_out = parse_redirection(node->cmd->redir_out, token);
	}
	return node;
}
