#include "../includes/minishell.h"

char	*ft_readline(char const *prompt, bool interactive)
{
	char	*line;

	line = NULL;
	if (interactive == true)
	{
		line = readline(prompt);
		return (line);
	}
	// line = get_next_line(0);
	return (line);
}

int	shell_loop(t_shell *shell)
{
	char		*line;
	t_token		*token_list;
	t_ast		*ast;
	t_result	*res;

	line = NULL;
	token_list = NULL;
	ast = NULL;
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

		token_list = lexer(line);

		t_token *cur = token_list;
		t_ast **ast_list = realloc(ast_list, sizeof(t_ast *)*token_list->count_newline);//count newline and keep it to token_list head.
		size_t i = cur->count_newline-1;
		while (i > 0)
		{
			ast = parser(&cur);
			ast_list[i] = ast;
			i--;
		}
		while (i < cur->count_newline)
		{
			res = executor(ast_list[i], shell);
			if (ast_list[i])
				free_ast_tree(ast_list[i]);
			if (res)
				free_result(res);
			i++;
		}
		xfree(line);
		if (token_list)
			free_token_list(token_list);
		xfree(ast_list);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	memset(&shell, 0, sizeof(t_shell));
	if (isatty(STDIN_FILENO) == 1)
		shell.interactive = true;
	signal_initializer(shell.interactive);
	init_env_from_envp(&shell, env);
	shell_loop(&shell);
	free_env_list(shell.env_list);
	return (0);
}
