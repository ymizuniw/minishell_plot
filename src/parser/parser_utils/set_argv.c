#include "../../../includes/minishell.h"

void set_argv(char **argv, t_token *token, size_t i)
{
	if (token->type==TK_DOLLER)
		argv[i] = '$';
	else
		argv[i] = expand_value(token);
	if (token->prev->type==TK_WORD || token->prev->type==TK_DOLLER)
		set_argv(argv, token->prev, i+1);
}
