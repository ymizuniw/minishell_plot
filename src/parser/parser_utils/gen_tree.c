#include "../../../includes/minishell.h"

static int	is_operator(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_PIPE || type == TK_AND_IF
		|| type == TK_OR_IF || type == TK_EOF)
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

// --------------------------------------------------
// classify the type of node based on token type
// --------------------------------------------------
void set_node_type(t_ast *node, t_token_type token_type)
{
	if (token_type == TK_AND_IF)
		node->type = NODE_AND;
	else if (token_type == TK_OR_IF)
		node->type = NODE_OR;
	else if (token_type == TK_PIPE)
		node->type = NODE_PIPE;
	else
		node->type = NODE_CMD;
}

t_ast *sort_by_logical_precedence(t_ast *parent, t_ast *node)
{
	t_ast	*root;

	root = parent;
	while (root && root->parent && (root->parent->type!=NODE_AND && root->parent->type!=NODE_OR))
		root = root->parent;
	node = swap_and_set_right_node(node, root);
	if (node==NULL)
	{
		printf("swap_and_set_right_node failed\n");
	}
	return (node);
}

int init_node(t_ast **node, t_ast *parent, t_token_type token_type)
{
	*node = alloc_node();
	if (!*node)
		return (-1);
	memset(*node, 0, sizeof(t_ast));
	set_node_type(*node, token_type);
	(*node)->parent = parent;
	return (1);
}

t_ast  *sort_and_gen_node(t_ast *parent, t_token **cur_token, t_token *next_token, int subshell)
{
	t_token *token = *cur_token;
	t_ast *node;

	if (init_node(&node, parent, token->type)<0)
		return (NULL);
	if (token->type == TK_AND_IF || token->type == TK_OR_IF)
		node = sort_by_logical_precedence(parent, node);
	else
		node = swap_and_set_right_node(node, parent);
	if (token && token->next)
	{
		next_token = token->next;
		*cur_token = next_token;
	}
	node->left = gen_tree(node, &next_token, subshell);
	if (node->left)
		node->left->parent = node;
	*cur_token = next_token;
	return (node);
}

t_ast  *gen_subshell_tree(t_ast *parent, t_token **cur_token, t_token *next_token, int subshell)
{
	t_token *token = *cur_token;
	t_ast *node;
	(void)subshell;
	if (init_node(&node, parent, token->type)<0)
		return (NULL);
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
	node->left = gen_tree(node, cur_token, subshell);
	return (node);
}

t_ast *subshell_close(t_ast *parent, t_token **cur_token, int subshell)
{

	t_token *token = *cur_token;
	t_ast *node;

	if (init_node(&node, parent, token->type)<0)
		return (NULL);
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

//parse_command_list

int init_redir(t_redir **redir, t_redir_type redir_type)
{
	*redir = alloc_redir();
	if (*redir==NULL)
		return (-1);
	memset(*redir, 0, sizeof(t_redir));
	(*redir)->type = redir_type;
	return (1);
}

int parse_redirection(t_ast *node, t_token **tmp, t_redir_type redir_type)
{
	t_redir *redir;

	if (!syntax_check(*tmp))
		return (-1);
	if (init_redir(&redir, redir_type)<0)
		return (-1);
	if ((*tmp)->prev)
		redir->filename = strdup((*tmp)->prev->value);
	if (!node->cmd->redir)
		node->cmd->redir = redir;
	else
	{
		t_redir *tmp_redir = node->cmd->redir;
		while (tmp_redir->next)
			tmp_redir = tmp_redir->next;
		tmp_redir->next = redir;
	}
	*tmp = (*tmp)->prev;
	return (1);
}

void parse_simple_command(t_ast *node, t_token **cur_token, size_t *i)
{
	set_argv(&node->cmd->argv, *cur_token, *i);
	(*i)++;
	*cur_token = (*cur_token)->prev;
}

int init_command_node(t_ast *parent, t_ast **node, t_token_type token_type)
{
	if (init_node(node, parent, token_type)<0)
		return (-1);
	(*node)->cmd = alloc_cmd();
	if (!(*node)->cmd)
		return (-1);
	memset((*node)->cmd, 0, sizeof(t_cmd));
	return (1);
}

int parse_redir_and_command(t_ast *node, t_token *tmp, t_token *cur_token)
{
	size_t i = 0;
	while (tmp)
	{
		t_redir_type type = get_redir_type(tmp->type);
		if (type != REDIR_OTHER)
		{
			if (parse_redirection(node, &tmp, type)<0)
				return (-1);
			continue ;
		}
		if (tmp->type == TK_WORD || tmp->type == TK_DOLLER)
		{
			parse_simple_command(node, &tmp, &i);
			continue ;
		}
	 	if (tmp==cur_token)
			break ;
		tmp = tmp->prev;
	}
	return(1);
}

void print_ast_info(t_ast *p)
{
	// printf("p->left:");
	// if (p->left!=NULL)
	// 	print_ast_info(p->left);
	printf("type: ");
	if (p->type == NODE_AND)
		printf("AND\n");
	else if (p->type == NODE_OR)
		printf("OR\n");
	else if (p->type == NODE_PIPE)
		printf("PIPE\n");
	else if (p->type == NODE_SUBSHELL)
		printf("SUBSHELL\n");
	else if (p->type == NODE_CMD)
		printf("CMD\n");
	printf("parent:");
	if (p->parent!=NULL)
		printf("exists\n");
	else
		printf("not exist\n");
}

t_ast *parse_command_list(t_ast *parent, t_token **cur_token, int subshell)
{
	t_token		*command_start;
	t_token		*tmp;
	t_ast 		*node;

	printf("token_value: %s\n", (*cur_token)->value);
	if (init_command_node(parent, &node, (*cur_token)->type)<0)
		return (NULL);
	command_start = *cur_token;
	// printf("command_start: %s\n", command_start->value);
	while (command_start->next && !is_operator(command_start->next->type))
		command_start = command_start->next;
	tmp = command_start;
	// printf("cur: %s\n", tmp->value);//right!
	if (parse_redir_and_command(node, tmp, *cur_token)<0)
		return (NULL);
	*cur_token = command_start->next;
	print_ast_info(node);
	node->left = gen_tree(node, cur_token, subshell);
	return (node);
}

t_ast	*gen_tree(t_ast *parent, t_token **cur_token, int subshell)
{
	t_token	*token;
	t_token	*next_token;

	if ((*cur_token)->type == TK_NEWLINE || (*cur_token)->type==TK_EOF)
		return (NULL);
	if (!cur_token || !*cur_token)
		return (NULL);
	token = *cur_token;
	next_token = NULL;

	t_ast *node;
	if (is_operator(token->type))
		node = sort_and_gen_node(parent, cur_token, next_token, subshell);
	else if (token->type == TK_RPAREN)
		node = gen_subshell_tree(parent, cur_token, next_token, subshell);
	else if (token->type == TK_LPAREN)
		node = subshell_close(parent, cur_token, subshell);
	else if (token->type == TK_WORD || token->type == TK_DOLLER)
		node = parse_command_list(parent, cur_token, subshell);
	else
		node = NULL;
	// while (node && node->parent)
	// 	node = node->parent;
	return (node);
}

//gen_tree() finally returns the root of the AST.