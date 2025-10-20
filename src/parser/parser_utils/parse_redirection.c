#include "../../../includes/minishell.h"

// create a list of redirection
t_redir	*parse_redirection(t_redir *redir, t_token *token)
{
	t_redir	*cur_redir;
	t_token	*cur_token;
	char	*file_name;

	cur_redir = redir;
	while (cur_redir != NULL)
		cur_redir = cur_redir->next;
	cur_redir = alloc_redir();
	bzero(cur_redir, sizeof(t_redir));
	if (token->type == TK_REDIRECT_IN)
	{
		if (token->next == NULL)
			return (NULL);
		file_name = expand_value(token->next);
		cur_redir->type = REDIR_IN;
		cur_redir->filename = file_name;
	}
	else if (token->type == TK_REDIRECT_OUT)
	{
		if (token->next == NULL)
			return (NULL);
		file_name = expand_value(token->next);
		cur_redir->type = REDIR_OUT;
		cur_redir->filename = file_name;
	}
	else if (token->type == TK_HEREDOC)
	{
		if (token->next == NULL)
			return (NULL);
		cur_redir->type = REDIR_HEREDOC;
		cur_redir->filename = token->next->value;
		if (token->next->in_dquote || token->next->in_squote)
			cur_redir->delim_quoted = true;
	}
	else if (token->type == TK_APPEND)
	{
		if (token->next == NULL)
			return (NULL);
		file_name = expand_value(token->next);
		cur_redir->type = REDIR_APPEND;
		cur_redir->filename = file_name;
	}
	if (redir == NULL)
		redir = cur_redir;
	cur_token = token->next->next;
	if (cur_token && cur_token->type != TK_HEAD
		&& (cur_token->type == TK_REDIRECT_IN
			|| cur_token->type == TK_REDIRECT_OUT
			|| cur_token->type == TK_HEREDOC || cur_token->type == TK_APPEND))
	{
		cur_redir->next = parse_redirection(cur_redir->next, cur_token);
	}
	return (redir);
}
