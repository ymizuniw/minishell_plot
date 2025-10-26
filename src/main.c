#include "../includes/minishell.h"

char *ft_readline(char const *prompt, bool interactive)
{
	char *line = NULL;
	if (interactive==true)
	{
		line = readline(prompt);
		return (line);
	}
	// line = get_next_line(0);
	return (line);
}

int	shell_loop(t_shell *shell)
{
	char		*line=NULL;
	t_token		*tokens=NULL;
	t_ast		*ast=NULL;

	while (1)
	{
		line = ft_readline("minishell$ ", shell->interactive);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (shell->interactive && *line)
			add_history(line);
		tokens = lexer(line);
		ast = parser(tokens);
		t_result *res = executor(ast, shell);
		xfree(line);
		if (tokens)
			free_token_list(tokens);
		if (ast)
			free_ast_tree(ast);
		if (res)
			free_result(res);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	t_shell shell;

	bzero(&shell, sizeof(t_shell));
	if(isatty(STDIN_FILENO)==1)
		shell.interactive = true;
	signal_initializer(shell.interactive);
	init_env_from_envp(&shell,env);
	shell_loop(&shell);
	free_env_list(shell.env_list);
	return (0);
}
