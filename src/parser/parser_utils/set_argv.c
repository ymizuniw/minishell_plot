#include "../../../includes/minishell.h"

// expand the value of the token and set to cmd list's argv.
void	set_argv(char **argv, t_token *token, size_t i)
{
	argv = realloc(argv, sizeof(char *) * (i + 2));
	if (!argv)
	{
		perror("realloc");
		return;
	}
	if (token->type == TK_DOLLER)
		(argv)[i] = strdup("$");
	else
		(argv)[i] = expand_value(token);
	(argv)[i + 1] = NULL;
}
