#include "../../../includes/minishell.h"

//create a list of redirection
t_redir	*parse_redirection(t_redir *redir, t_token *token)
{
	t_redir	*cur_redir;
	t_token	*cur_token;

	cur_redir = redir;
	while (cur_redir != NULL)
		cur_redir = cur_redir->next;
	cur_redir = alloc_redir();
	bzero(cur_redir, sizeof(t_redir));
	if (token->type==TK_REDIRECT_IN)
	{
        if (token->next==NULL)
            return ;
		char *file_name = expand_value(token);
		redir->type=REDIR_IN;
		redir->filename=file_name;
	}
	else if (token->type==TK_REDIRECT_OUT)
	{
		char *file_name = expand_value(token);
		redir->type=REDIR_OUT;
		redir->filename=file_name;
	}
	else if (token->type==TK_HEREDOC)
	{
		char *delim = token->next->value;
		redir->type=REDIR_HEREDOC;
		redir->filename=delim;
		if (token->next->in_dquote || token->next->in_squote)
			redir->delim_quoted = true;//if quoted, the $var won't be expanded in here-document.
	}
	else if (token->type==TK_APPEND)
	{
		char *file_name = expand_value(token);
		redir->type=REDIR_APPEND;
		redir->filename=file_name;
	}
	cur_token = token->next->next;
	if (cur_token->type != TK_HEAD && cur_token->type == TK_REDIRECT_IN
		|| cur_token->type == TK_REDIRECT_OUT || cur_token->type == TK_HEREDOC
		|| cur_token->type == TK_APPEND)
		return (redirection(cur_redir, cur_token));
	return (redir);
}