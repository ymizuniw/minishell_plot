// command search and expansion
#include "../../includes/minishell.h"

// If no command name results, redirections are performed,
	// but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.

//&& creates subshell
// evaluate previous command's return value,
// and if value is zero, do the right command,
// or skip the right command(subshell).
// redirections are not also done.

//&& creates subshell
// evaluate previous command's return value,
// and if value is non-zero, do the right command,
// or skip the right command(subshell).
// redirections are not also done.

// cmd1 | cmd2
// delegate STDOUT_FILENO of cmd1 to cmd2
// even if cmd1 fails, redirection and cmd2 will be executed.
// in case of it, the STDIN_FILENO of cmd2 is dev/null.
// ls [no-arg] : cwd
// ls [""]   : try to find and err.
// ls [arg-list-of-filename] : try to find the name and display the name.
// ls [arg-list-of-dirname]  : try to find the name and display the content.

// find and create a pipeline first. node shall have a flag int pipeline;

// if (node->type==NODE_CMD && node->pipeline==true)
// apply redirection between commands and fork();

// STDIN_FILENO, STDOUT_FILENO, stderr.
// dup2();
// or operator nodes are skipped?

// STDOUT_FILENO redirection are done,
	// so this node should inherit the types to the next node or leaf.

int	exec_plot(t_ast *node, char **env, int last_exit_status)
{
	pid_t	pid;
	int		exit_status;
	const char *path = path_validation(node->cmd->argv[0], env);

	exit_status = 0;
	if (is_operator(node->type))
	{
		if (node->type == NODE_AND)
		{
			if (last_exit_status == 0)
			{
				exec_plot(node->right, env, last_exit_status);
			}
			else
				return (last_exit_status);
		}
		else if (node->type == NODE_OR)
		{
			if (last_exit_status != 0)
			{
				exit_status = exec_plot(node->right, env, last_exit_status);
				if (exit_status == 0)
					return (exit_status);
				else
				{
					// non-zero exit return value.
				}
			}
			else
				return (last_exit_status);
		}
		else if (node->type == NODE_PIPE)
		{
			close(node->pipeline->out_fd);
			close(STDIN_FILENO);
			dup2(node->pipeline->in_fd, STDIN_FILENO);
			close(node->pipeline->in_fd);
			return (last_exit_status);
		}
		else if (node->type == NODE_SUBSHELL)
		{
			pid = fork();
			exit_status = ast_traversal(node->subtree, env);
			if (exit_status == 0)
				return (exit_status);
			else
			{
				// non-zero exit status cases.
			}
		}
	}
	// command
	if (node->type == NODE_CMD)
	{
		do_redirection(node);
		do_pipe_redirection(node);
		if (path == NULL)
		{
			// path null exit();
		}
		execve(path, node->cmd->argv, env);
	}
	return (exit_status);
}

// if pipe node comes, connect out to right node.
int	do_redirection(t_ast *node)
{
	t_redir	*cur;
			int fd;

	cur = node->cmd->redir;
	while (cur != NULL)
	{
		// redir condition branch
		if (cur->type == REDIR_IN)
		{
			fd = open(cur->filename, O_RDONLY);
			dup2(fd, stdin);
			close(fd);
		}
		else if (cur->type == REDIR_HEREDOC)
		{
			//gen exclusive tmpfile name.
			fd = open(cur->filename, O_WRONLY | O_APPEND | O_EXCL);
			if (cur->type == REDIR_IN)
			{
				dup2(fd, STDIN_FILENO);
				close(fd);
			}
		}
		else if (cur->type == REDIR_OUT)
		{
			fd = open(cur->filename, O_RDONLY | O_CREAT | O_TRUNC);
			if (cur->type == REDIR_IN)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
		else if (cur->type == REDIR_APPEND)
		{
			fd = open(cur->filename, O_WRONLY | O_CREAT |O_APPEND);
			if (cur->type == REDIR_IN)
			{
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}
		}
		cur = cur->next;
	}
	return (0);
}

int	do_pipe_redirection(t_ast *node)
{
	if (node->pipeline->in_pipeline)
	{
		dup2(node->pipeline->in_fd, STDIN_FILENO);
		close(node->pipeline->in_fd);
		dup2(node->pipeline->out_fd, STDOUT_FILENO);
		close(node->pipeline->out_fd);
	}
	else
	{
		close(node->pipeline->out_fd);
		close(STDIN_FILENO);
		dup2(node->pipeline->in_fd, STDIN_FILENO);
		close(node->pipeline->in_fd);
		close(node->pipeline->out_fd);
	}
}

int	ast_traversal(t_ast *node, char **env)
{
	t_ast	*cur;
	int		last_exit_status;
	pid_t	pid;

	if (cur==NULL)
		return (0);
	cur = node;
	last_exit_status = 0;
	if (cur->type == NODE_AND)
	{
		if (last_exit_status == 0)
			return (exec_plot(node->right, env, last_exit_status));
		else
			return (last_exit_status);
	}
	else if (cur->type == NODE_OR)
	{
		if (last_exit_status != 0)
			return (exec_plot(node->right, env, last_exit_status));
		else
			return (last_exit_status);
	}
	else if (cur->type==NODE_PIPE)
		last_exit_status = exec_pipe(node, env);//if cmd | (cmd), fork() is done three times?
	else if (cur->type==NODE_SUBSHELL)
		last_exit_status = exec_subshell(node, env);
	else
		last_exit_status = exec_command(node, env);
	return (last_exit_status);
}

int exec_pipe(t_ast *node, char **env)//just pipe()?
{
	int right_status=0;
	int left_status=0;
	int pip[2];//how about make this static?
	int ret = pipe(pipe);
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

int exec_subshell(t_ast *node, char **env)
{
	int status=0;
	pid_t pid = fork();
	if (pid<0)
		perror("fork :");
	int ret = ast_traversal(node, env);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status)==0)
		return (WEXITSTATUS(status));
	else
		return (EXIT_FAILURE);
	//go next node.
}

int exec_command(t_ast *node, char **env)
{
	pid_t pid = fork();
	if (pid<0)
		perror("fork :");
	
	int redir_ret = do_redirection(node);
	if (redir_ret<0)
	{
		perror(node->cmd->redir->filename);//redirection failure.
		exit(redir_ret);
	}
	char *path = validate_path(node->cmd->argv[0], env);
	if (path==NULL)
		exit(127);//command not found exit.
	int execve_ret = execve(path, node->cmd->argv, env);
	if (execve_ret<0)
		exit(1);
}
