#include "../../../includes/minishell.h"


// static void print_argv(char **argv)
// {
// 	size_t i=0;
// 	while(argv[i])
// 	{
// 		printf("argv[%zu]=%s\n", i, argv[i]);
// 		i++;
// 	}
// }

// expand the value of the token and set to cmd list's argv.
void	set_argv(char ***argv, t_token *token, size_t i)
{
	*argv = realloc(*argv, sizeof(char *) * (i + 2));
	if (!argv)
	{
		perror("realloc");
		return;
	}
	if (token->type == TK_DOLLER)
		(*argv)[i] = strdup("$");
	else
	{
		char *new = expand_value(token);
		// printf("new: %s\n", new);
		(*argv)[i] = strdup(new);
		free(new);
		new=NULL;
	}
	(*argv)[i + 1] = NULL;
}
