#include "../../../includes/minishell.h"

//check the syntax of given token is correct.
int	syntax_check(t_token *token)
{
	t_token_type	token_type;

	token_type = token->type;
	if (token_type == TK_LPAREN)
		return (0);
	if (token_type == TK_RPAREN)
		return (check_parenthesis(token));
	if (token_type == TK_REDIRECT_IN)
	{
        if (token->prev->type == TK_HEAD)
            return (0);
		if (token->prev->type == TK_NEWLINE)
		{
			syntax_error(TK_NEWLINE);
			return (0);
		}
		if (!(token->prev->type == TK_WORD))
            return (0);
	}
	if (token_type == TK_HEREDOC)
	{
		if (token->prev->type==TK_WORD)
		{
			syntax_error(token->prev->type);
			return (0);
		}
	}
	if (token_type == TK_REDIRECT_OUT)
	{
		if (!(token->prev->type == TK_WORD))
		{
			syntax_error(token->prev->type);
			return (0);
		}
	}
	if (token_type == TK_APPEND)
	{
		if (token->prev->type != TK_WORD)
		{
			syntax_error(token->prev->type);
			return (0);
		}
	}
	if (token_type == TK_REDIRECT_IN_AND_OUT)
	{
		if (token->prev->type != TK_WORD)
		{
			syntax_error(token->prev->type);
			return (0);
		}
	}
	return (1);
}
