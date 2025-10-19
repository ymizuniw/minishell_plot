#include "../../../includes/minishell.h"

//expand the value of the token and set to cmd list's argv.
void set_argv(char **argv, t_token *token, size_t i)
{
	if (!argv || !token)
		return;
	if (token->type == TK_DOLLER)
		argv[i] = strdup("$");
	else
		argv[i] = expand_value(token);
	if (token->prev!=NULL && (token->prev->type == TK_WORD || token->prev->type == TK_DOLLER))
		set_argv(argv, token->prev, i + 1);
}
