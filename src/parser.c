#include "minishell.h"

t_ast	*parser(t_token *token_head)
{
	t_token	*token;

	token = token_head->next;

	// logic and pipe
}

int parse_command(t_token *token, t_cmd *cmd)
{
	size_t i=0;
	
}

void set_argv(char **argv, t_token *token, size_t i)
{
	argv[i] = expand_value();
	if (token->prev->type==TK_WORD)
		set_argv(argv, token->prev, i+1);
}

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
	else if (token->type == TK_WORD)
	{
		size_t i = 0;
		node->type = NODE_CMD;
		while (token->next && token->type == TK_WORD || token->type==TK_REDIRECT_IN || token->type==TK_REDIRECT_OUT || token->type==TK_HEREDOC || token->type==TK_APPEND)
			token=token->next;
		t_token *keep_token = token;
		syntax_check(token);
		if (token->type == TK_WORD)
		{
			node->cmd->argv[i] = expand_value();
			if (token->prev->type==TK_WORD)
				set_argv(node->cmd->argv, token->prev, i+1);
		}
		if (token->type==TK_REDIRECT_IN || token->type==TK_REDIRECT_OUT || token->type==TK_HEREDOC || token->type==TK_APPEND)
			node->cmd->redir = redirection(node->cmd->redir, token);
	}
}
