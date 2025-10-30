#include "../../includes/minishell.h"

// Forward declarations to avoid implicit prototypes
int		token_is_redirection(t_token_type type);
int		token_is_operator(t_token_type type);
int		token_is_subshell_close(t_token_type type);
int		token_is_command(t_token_type type);
int		token_is_newline_or_eof(t_token_type type);
int		parse_redirection(t_redir **redir_head, t_token_type token_type,
			t_token *filename_token);
int		parse_simple_command(t_argv **argv_head, t_token *command_token);
int		parse_command_list(t_cmd *cmd, t_token **cur_token);
t_ast	*swap_with_parent(t_ast **parent, t_token **cur_token);
t_ast	*gen_subshell_node(t_ast *parent, t_token **cur_token);
t_ast	*gen_command_node(t_ast *parent, t_token **cur_token);
t_ast	*gen_eof_newline_node(t_ast *parnt, t_token **cur_token);
void	fgen_tree(t_ast **parent, t_token **cur_token);

// comprehensive tests should check

// parsing new node is done.
// cur_token is cunsumed correctly.
// AST has right structure after parsing.
// no leak.

#include "../../includes/minishell.h"

int	token_is_redirection(t_token_type type)
{
	if (type == TK_REDIRECT_IN || type == TK_REDIRECT_OUT || type == TK_HEREDOC
		|| type == TK_APPEND)
		return (1);
	return (0);
}

int	parse_redirection(t_redir **redir_head, t_token_type token_type,
		t_token *filename_token)
{
	t_redir	*new_redir;
	t_redir	*redir;

	redir = *redir_head;
	new_redir = alloc_redir();
	if (!new_redir)
		return (-1);
	memset(new_redir, 0, sizeof(t_redir));
	if (redir == NULL)
		*redir_head = new_redir;
	else
	{
		while (redir->next)
			redir = redir->next;
		redir->next = new_redir;
	}
	if (token_type == TK_REDIRECT_IN)
		new_redir->type = REDIR_IN;
	else if (token_type == TK_REDIRECT_OUT)
		new_redir->type = REDIR_OUT;
	else if (token_type == TK_HEREDOC)
		new_redir->type = REDIR_HEREDOC;
	else
		new_redir->type = REDIR_APPEND;
	new_redir->filename = strdup(filename_token->value);
	if (new_redir->type == REDIR_HEREDOC)
	{
		if (filename_token->in_dquote || filename_token->in_squote)
			new_redir->delim_quoted = true;
	}
	return (1);
}

// typedef struct s_argv
// {
// 	char			*word;
// 	bool			to_expand;
// 	struct s_argv	*next;
// }					t_argv;

int	parse_simple_command(t_argv **argv_head, t_token *command_token)
{
	t_argv	*new_argv;
	t_argv	*argv;

	argv = *argv_head;
	new_argv = alloc_argv();
	if (!new_argv)
		return (-1);
	memset(new_argv, 0, sizeof(t_argv));
	if (*argv_head == NULL)
		*argv_head = new_argv;
	else
	{
		while (argv->next)
			argv = argv->next;
		argv->next = new_argv;
	}
	new_argv->word = strdup(command_token->value);
	// TK_DOLLER is single '$' print token.
	if (command_token->in_squote == false && command_token->type != TK_DOLLER)
		new_argv->to_expand = true;
	return (1);
}

int	parse_command_list(t_cmd *cmd, t_token **cur_token)
{
	t_token	*tmp;
	int		parse_success;

	parse_success = 0;
	tmp = *cur_token;
	printf("[parse_command_list] start cur=%p type=%d\n", (void *)tmp,
		tmp ? (int)tmp->type : -1);
	while (tmp->next && token_is_command(tmp->next->type))
		tmp = tmp->next;
	// Move tmp to the rightmost command token first (since tokens are reversed).
	while (token_is_command(tmp->type) || token_is_redirection(tmp->type))
	{
		if (token_is_redirection(tmp->type))
		{
			parse_success = parse_redirection(&cmd->redir, tmp->type,
					tmp->prev);
			tmp = tmp->prev;
		}
		else
			parse_success = parse_simple_command((t_argv **)&cmd->argv, tmp);
		if (parse_success == 0)
		{
			printf("parse %s failed\n", tmp->value);
			return (-1);
		}
		tmp = tmp->prev;
	}
	// After parsing the contiguous command/redirect sequence, expose the
	// next token for the caller so it can continue from the correct position
	*cur_token = tmp;
		// hand back the first non-command/non-redir token to the left
	printf("[parse_command_list] end next cur=%p type=%d\n", (void *)*cur_token,
		*cur_token ? (int)(*cur_token)->type : -1);
	return (1);
}

int	token_is_operator(t_token_type type)
{
	if (type == TK_AND_IF || type == TK_OR_IF || type == TK_PIPE)
		return (1);
	return (0);
}

int	token_is_subshell_close(t_token_type type)
{
	if (type == TK_LPAREN)
		return (1);
	return (0);
}

int	token_is_command(t_token_type type)
{
	if (type == TK_WORD || type == TK_DOLLER)
		return (1);
	return (0);
}

int	token_is_newline_or_eof(t_token_type type)
{
	if (type == TK_NEWLINE || type == TK_EOF)
		return (1);
	return (0);
}

t_ast	*swap_with_parent(t_ast **parent, t_token **cur_token)
{
	t_ast	*node;

	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	if ((*cur_token)->type == TK_AND_IF)
		node->type = NODE_AND;
	else if ((*cur_token)->type == TK_OR_IF)
		node->type = NODE_OR;
	else if ((*cur_token)->type == TK_PIPE)
		node->type = NODE_PIPE;
	node->right = *parent;
	(*parent)->parent = node;
	return (node);
}

t_ast	*gen_subshell_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;
	t_ast	*subroot;

	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_SUBSHELL;
	subroot = NULL;
	fgen_tree(&subroot, cur_token);
	node->subtree = subroot;
	return (node);
}

t_ast	*gen_command_node(t_ast *parent, t_token **cur_token)
{
	t_ast	*node;

	node = alloc_node();
	if (!node)
		return (NULL);
	memset(node, 0, sizeof(t_ast));
	node->parent = parent;
	node->type = NODE_CMD;
	node->cmd = alloc_cmd();
	if (!node->cmd)
		return (NULL);
	memset(node->cmd, 0, sizeof(t_cmd));
	parse_command_list(node->cmd, cur_token);
	return (node);
}

// end of command or command line.
t_ast	*gen_eof_newline_node(t_ast *parnt, t_token **cur_token)
{
	t_ast	*node;

	(void)parnt;
	(void)cur_token;
	node = NULL;
	return (node);
}

// this is not for real usage.
int	syntax_check(t_token *token)
{
	(void)token;
	return (1);
}

void	fgen_tree(t_ast **parent, t_token **cur_token)
{
	t_ast	*left;
	t_ast	*node;

	if (!*cur_token || token_is_newline_or_eof((*cur_token)->type))
		return ;
	printf("[fgen_tree] enter cur=%p type=%d parent=%p\n", (void *)*cur_token,
		(int)(*cur_token)->type, (void *)*parent);
	left = NULL;
	node = NULL;
	if (token_is_operator((*cur_token)->type))
		node = swap_with_parent(parent, cur_token);
	else if (token_is_subshell_close((*cur_token)->type)
		&& syntax_check(*cur_token))
		node = gen_subshell_node(*parent, cur_token);
	else if (token_is_command((*cur_token)->type))
		node = gen_command_node(*parent, cur_token);
	else
		node = gen_eof_newline_node(*parent, cur_token);
	if (node == NULL)
		return ;
	*parent = node;
	printf("[fgen_tree] built node=%p type=%d; recurse left with cur=%p type=%d\n",
		(void *)node, (int)node->type, (void *)*cur_token,
		(int)(*cur_token)->type);
	fgen_tree(&left, cur_token);
	(*parent)->left = left;
	printf("[fgen_tree] exit node=%p left=%p right=%p subtree=%p\n",
		(void *)*parent, (void *)(*parent)->left, (void *)(*parent)->right,
		(void *)(*parent)->subtree);
}
