#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	(void)argc; (void)argv;
	printf("=== env management test ===\n");
	t_shell shell; bzero(&shell, sizeof(shell));
	init_env_from_envp(&shell, envp);
	assert(shell.env_list != NULL);
	// find PATH or HOME optionally
	if (find_env(shell.env_list, "PATH"))
		printf("PATH found\n");
	free_env_list(shell.env_list);
	printf("ok\n");
	return 0;
}
