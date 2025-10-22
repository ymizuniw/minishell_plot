#include "../../../includes/minishell.h"

// create a list of redirection. tail_token is cpy of tail_token of caller.
t_redir	*parse_redirection(t_redir *redir, t_token **tail_token)
{
	t_redir	*cur_redir;
	t_token	*cur_token;
	t_token *token = *tail_token;
	char	*file_name;
	t_token *next = token->next;

	if (next->type == TK_REDIRECT_IN || next->type == TK_REDIRECT_OUT
		|| next->type == TK_HEREDOC || next->type == TK_APPEND)
	{
		syntax_check(next);
		//set redir.
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
		redir->next = cur_redir;
		*tail_token = (*tail_token)->next->next->next;
	}
	if (*tail_token && (*tail_token)->type==TK_WORD || (*tail_token)->type==TK_DOLLER)
		return (parse_redirection(cur_redir, tail_token));
	return (redir);
}
