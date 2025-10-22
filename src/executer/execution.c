#include "../../includes/minishell.h"

int	do_redirection(t_ast *node)
{
	t_redir	*cur;
			int fd;

	cur = node->cmd->redir;
	while (cur != NULL)
	{
		if (cur->type == REDIR_IN)
		{
			fd = open(cur->filename, O_RDONLY);
			dup2(fd, stdin);
			close(fd);
		}
		else if (cur->type == REDIR_HEREDOC)
		{
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

int	ast_traversal(t_ast *node, char **env)
{
	t_ast	*cur;
	int		last_exit_status;
	pid_t	pid;

	if (cur==NULL)
		return (0);
	cur = node;
	last_exit_status = 0;
	if (cur->type==NODE_AND)
	{
		last_exit_status = ast_traversal(node->left, env);
		if (last_exit_status==0)
			last_exit_status = ast_traversal(node->right, env);
	}
	else if (cur->type==NODE_OR)
	{
		last_exit_status = ast_traversal(node->left, env);
		if (last_exit_status!=0)
			last_exit_status = ast_traversal(node->right, env);
	}
	else if (cur->type==NODE_PIPE)
		last_exit_status = exec_pipe(node, env);//if cmd | (cmd), fork() is done three times?
	else if (cur->type==NODE_SUBSHELL)
		last_exit_status = exec_subshell(node, env);
	else
		last_exit_status = exec_command(node, env);
	return (last_exit_status);
}

//if node->type==NODE_PIPE, pipe() and fork() left and right node.
//the return value is the return value of ast_traversal() recursively called here when in child process,
//or return the exit status of lastly executed command.
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

//conduct redirection and execute command.
int exec_command(t_ast *node, char **env)
{
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
