#include "../includes/minishell.h"

char	*ft_readline(char const *prompt, bool interactive)
{
	char	*line;
	size_t	len;
	ssize_t	nread;

	line = NULL;
	len = 0;
	if (interactive == true)
	{
		line = readline(prompt);
		return (line);
	}
	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	if (nread > 0 && line[nread - 1] == '\n')
		line[nread - 1] = '\0';
	return (line);
}

static void	exec_one_ast(t_ast *ast, t_shell *shell)
{
	t_result	*res;

	if (!ast)
		return ;
	res = executor(ast, shell);
	if (res)
	{
		shell->last_exit_status = res->exit_code;
		free_result(res);
	}
	free_ast_tree(ast);
}

int	parse_and_exec(t_token *token_list, t_shell *shell)
{
	t_token	*cur;
	t_ast	*ast;

	if (!token_list || !shell)
		return (0);
	cur = token_list;
	while (cur && cur->type != TK_EOF)
	{
		fprintf(stderr, "DEBUG parse_and_exec: cur=%p type=%d\n", (void *)cur,
			cur->type);
		// Skip HEAD and NEWLINE tokens
		if (cur->type == TK_HEAD || cur->type == TK_NEWLINE)
		{
			cur = cur->next;
			continue ;
		}
		// Parse one command, advancing cur to next unconsumed token
		ast = parser(&cur);
		fprintf(stderr,
			"DEBUG parse_and_exec: parser returned ast=%p,cur->type=%d\n",
			(void *)ast, cur ? (int)cur->type : -1);
		if (ast)
			exec_one_ast(ast, shell);
		// cur now points to NEWLINE or EOF after parser advanced it
	}
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
		fprintf(stderr, "DEBUG: Read line: %s (interactive=%d)\n",
			line ? line : "(null)", shell->interactive);
		if (!line)
		{
			if (shell->interactive)
				printf("exit\n");
			break ;
		}
		if (shell->interactive && *line)
			add_history(line);
		token_list = lexer(line);
		fprintf(stderr, "DEBUG: Token list created: %p\n", (void *)token_list);
		parse_and_exec(token_list, shell);
		fprintf(stderr, "DEBUG: After parse_and_exec, exit_status=%d\n",
			shell->last_exit_status);
		xfree(line);
		if (token_list)
			free_token_list(token_list);
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_shell	shell;
	char	*pwd;

	(void)argc;
	(void)argv;
	memset(&shell, 0, sizeof(t_shell));
	if (isatty(STDIN_FILENO) == 1)
		shell.interactive = true;
	signal_initializer(shell.interactive);
	init_env_from_envp(&shell, env);
	pwd = getcwd(NULL, 0);
	if (pwd)
	{
		shell.pwd = pwd;
		set_variable(&shell, "PWD", pwd, 1);
	}
	shell.last_exit_status = 0;
	shell_loop(&shell);
	if (shell.pwd)
		free(shell.pwd);
	free_env_list(shell.env_list);
	return (shell.last_exit_status);
}
