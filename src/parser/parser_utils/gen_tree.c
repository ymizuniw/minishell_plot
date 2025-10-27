#include "../../../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_PIPE || type == TK_AND_IF
		|| type == TK_OR_IF)
		return (1);
	return (0);
}

t_redir_type	get_redir_type(t_token_type token_type)
{
	if (token_type == TK_REDIRECT_IN)
		return (REDIR_IN);
	if (token_type == TK_REDIRECT_OUT)
		return (REDIR_OUT);
	if (token_type == TK_HEREDOC)
		return (REDIR_HEREDOC);
	if (token_type == TK_APPEND)
		return (REDIR_APPEND);
	return (REDIR_OTHER);
}

t_ast	*swap_and_set_right_node(t_ast *new_parent, t_ast *old_parent)
{
	if (!new_parent)
		return (old_parent);
	if (!old_parent)
		return (new_parent);
	new_parent->right = old_parent;
	old_parent->parent = new_parent;
	return (new_parent);
}

// -----------------------------------------------------------------------------
// generate a tree of command.
// -----------------------------------------------------------------------------
t_ast	*gen_tree(t_ast *parent, t_token **cur_token, int subshell)
{
	t_ast	*node;
	t_token	*token;
	t_token	*next_token;
	t_ast	*root;

	if (!cur_token || !*cur_token)
		return (NULL);

	token = *cur_token;
	next_token = NULL;

	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));

	// --------------------------------------------------
	// classify the type of node based on token type
	// --------------------------------------------------
	if (token->type == TK_AND_IF)
		node->type = NODE_AND;
	else if (token->type == TK_OR_IF)
		node->type = NODE_OR;
	else if (token->type == TK_PIPE)
		node->type = NODE_PIPE;
	else
		node->type = NODE_CMD;

	node->parent = parent;

	// --------------------------------------------------
	// operator case: &&, ||, |
	// --------------------------------------------------
	//set parent node as right node of the operator. Also, if it is a logical operator token,
	//set the root node of the current operator's right node.
	if (is_operator(token->type))
	{
		if (token->type == TK_AND_IF || token->type == TK_OR_IF)
		{
			root = parent;
			while (root && root->parent)
				root = root->parent;
			node = swap_and_set_right_node(node, root);
		}
		else
			node = swap_and_set_right_node(node, parent);

		if (token && token->next)
		{
			*cur_token = token->next;
			next_token = *cur_token;
		}
		node->left = gen_tree(node, &next_token, subshell);
		if (node->left)
			node->left->parent = node;
		*cur_token = next_token;
		return (node);
	}

	// --------------------------------------------------
	// subshells
	// --------------------------------------------------
	//if right parenthesis comes, it is the start of the subshell. do syntax check and generate subshell tree from the node->subtree.
	//set subshell flag to 1.
	else if (token->type == TK_RPAREN)
	{
		node->type = NODE_SUBSHELL;
		if (syntax_check(token) != 1)
			return (NULL);
		*cur_token = token->next;
		next_token = *cur_token;
		node->subtree = gen_tree(NULL, &next_token, 1);
		if (!node->subtree)
		{
			free(node);
			return (NULL);
		}
		*cur_token = next_token;
		return (node);
	}
	//if left parenthesis comes, it is the end of the subshell, or invalid token. 
	else if (token->type == TK_LPAREN)
	{
		if (subshell == 1)
		{
			*cur_token = token->next;
			return (node);
		}
		else
		{
			syntax_error(TK_RPAREN);
			return (NULL);
		}
	}

	// --------------------------------------------------
	// command (WORD / $)
	// --------------------------------------------------
	//if word token or doller token comes, parse redirection and command. to process them in order, proceed token to the end of the command_list,
	//and 
	else if (token->type == TK_WORD || token->type == TK_DOLLER)
	{
		size_t		i = 0;
		t_token		*command_end;
		t_token		*cur;

		node->cmd = alloc_cmd();
		memset(node->cmd, 0, sizeof(t_cmd));

		// find the last token before operator
		command_end = token;
		while (command_end->next && !is_operator(command_end->next->type))
			command_end = command_end->next;

		cur = token;
		while (cur && cur != command_end->next)
		{
			t_redir_type type = get_redir_type(cur->type);

			// -----------------------------
			// handle redirections
			// -----------------------------
			if (type != REDIR_OTHER)
			{
				if (!syntax_check(cur))
					return (NULL);

				t_redir *redir = alloc_redir();
				memset(redir, 0, sizeof(t_redir));
				redir->type = type;

				if (cur->next)
					redir->filename = strdup(cur->next->value);

				// append to list
				if (!node->cmd->redir)
					node->cmd->redir = redir;
				else
				{
					t_redir *tmp = node->cmd->redir;
					while (tmp->next)
						tmp = tmp->next;
					tmp->next = redir;
				}

				cur = cur->next ? cur->next->next : NULL;
				continue ;
			}

			// -----------------------------
			// handle argv words
			// -----------------------------
			if (cur->type == TK_WORD || cur->type == TK_DOLLER)
			{
				set_argv(&node->cmd->argv, cur, i);
				i++;
				cur = cur->next;
				continue ;
			}
			cur = cur->next;
		}
		*cur_token = command_end->next;
	}
	return (node);
}
