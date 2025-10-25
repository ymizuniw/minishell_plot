#include "../includes/minishell.h"

int	shell_loop(char **env)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	t_shell 	shell;
	init_env_from_envp(&shell,env);
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line)
			add_history(line);
		tokens = lexer(line);
		ast = parser(tokens);
		t_result *res = executor(ast, &shell);
		free(line);
		if (tokens)
			free_token_list(tokens);
		if (ast)
			free_ast_tree(ast);
		if (res)
			free_result(res);
	}
	free_env_list(shell.env_list);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	shell_loop(env);
	return (0);
}
