#include "../../../includes/minishell.h"

//gen_command_tree
t_ast	*gen_command_list(t_ast *ast, t_token *token)
{
	t_ast			*node;
	size_t			consumed_token_count;

	node = alloc_node();
	bzero(node, sizeof(t_ast));
	if (is_operator(token->type))
	{
		if (token->type == TK_AND_IF)
			node->type = NODE_AND;
		else if (token->type == TK_OR_IF)
			node->type == NODE_OR;
		else if (token->type == TK_PIPE)
			node->type == NODE_PIPE;
		swap_and_set_right_node(node, node->parent);
		node->left = gen_command_list(node->left, token->next);
	}
	else if (token->type==TK_RPAREN)
	{
		node->type = NODE_SUBSHELL;
		if (!syntax_check(token))
			syntax_err();
		node->subtree = gen_subtree(node->subtree, token,
				&consumed_token_count);
		while (consumed_token_count > 0)
		{
			token = token->next;
			consumed_token_count--;
		}
	}
	else if (token->type == TK_LPAREN)
	{
		//parenthesis syntax err.
	}
	else if (token->type == TK_WORD || token->type==TK_DOLLER)
	{
		size_t i = 0;
		node->type = NODE_CMD;
		while (token->next && token->type == TK_WORD || token->type==TK_REDIRECT_IN || token->type==TK_REDIRECT_OUT || token->type==TK_HEREDOC || token->type==TK_APPEND || token->type==TK_DOLLER)
			token=token->next;
		t_token *keep_token = token;
		syntax_check(token);
		if (token->type==TK_REDIRECT_IN || token->type==TK_REDIRECT_OUT || token->type==TK_HEREDOC || token->type==TK_APPEND)
			node->cmd->redir_in = redirection(node->cmd->redir_in, token);
		if (token->type == TK_WORD || token->type == TK_DOLLER)
		{
			node->cmd->argv = realloc(node->cmd->argv, sizeof(char *)*(i+3));
			node->cmd->argv[i] = expand_value(token);
			if (token->prev->type==TK_WORD || token->prev->type==TK_DOLLER)
				set_argv(node->cmd->argv, token->prev, i+1);
		}
		if (token->type==TK_REDIRECT_IN || token->type==TK_REDIRECT_OUT || token->type==TK_HEREDOC || token->type==TK_APPEND)
			node->cmd->redir_out = redirection(node->cmd->redir_out, token);
	}
	else
	{
		syntax_err();
	}
}