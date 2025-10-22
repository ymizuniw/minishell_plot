#include "../../../../includes/minishell.h"

//fork() and call ast_traversal for subtree.
int exec_subshell(t_ast *subtree, char **env)
{
	int status=0;
	pid_t pid = fork();
	if (pid<0)
		perror("fork :");
	int ret = ast_traversal(subtree, env);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status)==0)
		return (WEXITSTATUS(status));
	else
		return (EXIT_FAILURE);
	//go next node.
}
