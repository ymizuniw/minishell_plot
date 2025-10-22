#include "../includes/minishell.h"

void	cpy_env(char ***env, char **envp)
{
	size_t	env_size;

	if (envp == NULL || *envp == NULL)
		return ;
	env_size = 0;
	while (envp[env_size])
		env_size++;
	*env = malloc(sizeof(char *) * env_size);
	bzero(*env, sizeof(char *) * env_size);
	while (env_size-- > 0)
		(*env)[env_size] = env[env_size];
}

int	shell_loop(char **envp)
{
	char		*line;
	t_token		*tokens;
	t_ast		*ast;
	t_result	*res;
	char		**env;

	env = NULL;
	cpy_envp_to_env(&env, envp);
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
		// free result and ast elements.
	}
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	shell_loop(env);
	return (0);
}
