#include "../../../../includes/minishell.h"

int exec_pipe(t_ast *node, char **env)//just pipe()?
{
	int right_status=0;
	int left_status=0;
	int pip[2];//how about make this static?
	int ret = pipe(pip);
	if (ret<0)
		perror("pipe :");
	node->pipeline->in_fd = pip[0];
	node->pipeline->out_fd = pip[1];
	pid_t left_pid = fork();
	if (left_pid<0)
		perror("fork: ");
	pid_t right_pid = fork();
	if (right_pid<0)
		perror("fork: ");
	if (left_pid==0)
	{
		dup2(pip[0], STDIN_FILENO);
		dup2(pip[1], STDOUT_FILENO);
		close(pip[0]);
		close(pip[1]);
		return (ast_traversal(node->left, env));
	}
	if (right_pid==0)
	{
		dup2(pip[0], STDIN_FILENO);
		dup2(pip[1], STDOUT_FILENO);
		close(pip[0]);
		close(pip[1]);
		return (ast_traversal(node->right, env));
	}
	close(pip[0]);
	close(pip[1]);
	waitpid(left_pid, &left_status,0);
	waitpid(right_pid, &right_status, 0);
	if (WIFEXITED(right_status)==0)
		return (WEXITSTATUS(right_status));
	else
		return (EXIT_FAILURE);
	//go next node.
}