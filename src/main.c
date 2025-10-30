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
	return (line);
}

int	parse_and_exec(t_token *token_list, t_shell *shell)
{
	t_ast		**ast_list;
	t_ast		*ast;
	t_result	*res;
	size_t		ast_count;
	t_token		*cur;
	size_t		i;

	ast_list = NULL;
	ast = NULL;
	res = NULL;
	ast_count = 0;
	cur = token_list;
	while (cur && cur->type != TK_EOF)
	{
		ast = parser(cur);
		if (!ast)
			continue ;
		ast_list = realloc(ast_list, sizeof(t_ast *) * (ast_count + 1));
		ast_list[ast_count++] = ast;
		while (cur && cur->type == TK_NEWLINE)
			cur = cur->next;
	}
	i = 0;
	while (i < cur->count_newline)
	{
		res = executor(ast_list[i], shell);
		free_ast_tree(ast_list[i]);
		free_result(res);
		i++;
	}
	xfree(ast_list);
	return (1);
}

int	shell_loop(t_shell *shell)
{
	char	*line;
	t_token	*token_list;

	line = NULL;
	token_list = NULL;
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
		parse_and_exec(token_list, shell);
		xfree(line);
		if (token_list)
			free_token_list(token_list);
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
