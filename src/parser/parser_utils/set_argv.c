#include "../../../includes/minishell.h"

// expand the value of the token and set to cmd list's argv.
void	set_argv(char **argv, t_token **start_token, size_t i)
{
	if (!argv || !start_token || !*start_token)
		return ;
	t_token *token = *start_token;

	if (token->next!=NULL && token->next->type==TK_REDIRECT_IN || token->next==TK_REDIRECT_OUT || token->next==TK_HEREDOC || token->next==TK_APPEND)
	{
		*start_token=token->next->next;
		token = *start_token;
	}
	if (token->next != NULL && (token->next->type == TK_WORD
			|| token->next->type == TK_DOLLER))
	{
		*start_token = token->next;
		set_argv(argv, start_token, i + 1);
	}
	argv = realloc(argv, sizeof(char *)*(i+2));
	if (argv==NULL)
		perror("realloc :");
	if (token->type == TK_DOLLER)
		argv[i] = strdup("$");
	else
		argv[i] = expand_value(token);
}
