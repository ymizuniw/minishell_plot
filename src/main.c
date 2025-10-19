#include "../includes/minishell.h"

int	shell_loop(char **env)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	t_result	*res;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = lexer(line); // formant analisis.
		ast = parser(tokens); // abstract structure tree.
		res = executor(ast, env);
		free(line);
		//free result and ast elements.
	}
	return (0);
}

int main(int argc, char **argv, char **env)
{
	shell_loop(env);
	return(0);
}
