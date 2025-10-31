#include "../../../../includes/minishell.h"

// the lack of close failure perror and trying to close other fd.

int	exec_pipe(t_ast *node, t_shell *shell)
{
	int		pip[2];
	int		ret;
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node || !shell)
		return (0);
	ret = pipe(pip);
	if (ret < 0)
	{
		perror("pipe");
		return (1);
	}
	right_pid = fork();
	if (right_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		return (-1);
	}
	if (right_pid == 0)
	{
		set_sig_dfl();
		close(pip[0]);
		if (dup2(pip[1], STDOUT_FILENO) < 0)
			ft_exit(node->cmd->argv, 1, shell);
		close(pip[1]);
		ast_traversal(node->right, shell);
		ft_exit(node->cmd->argv, shell->last_exit_status, shell);
	}
	left_pid = fork();
	if (left_pid < 0)
	{
		perror("fork");
		close(pip[0]);
		close(pip[1]);
		kill(right_pid, SIGTERM);
		waitpid(right_pid, NULL, 0);
		return (-1);
	}
	if (left_pid == 0)
	{
		set_sig_dfl();
		close(pip[1]);
		if (dup2(pip[0], STDIN_FILENO) < 0)
			ft_exit(node->cmd->argv, shell->last_exit_status, shell);
		close(pip[0]);
		ast_traversal(node->left, shell);
		ft_exit(node->cmd->argv, shell->last_exit_status, shell);
	}
	close(pip[0]);
	close(pip[1]);
	handle_child(&shell->last_exit_status, right_pid);
	handle_child(&shell->last_exit_status, left_pid);
	return (0);
}
