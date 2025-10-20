// command search and expansion
#include "../includes/minishell.h"

// If no command name results, redirections are performed,
// but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.

int	search_built_in_command(char *cmd_name)
{
	char	*path;

	path = getenv(cmd_name);
}

int	exec_plot(t_ast *node)
{
	do_redirections(node->redir);
	if (command_name_with_no_slash)
	{
		if (search_built_in_command())
		{
			command_invoke();
		}
		else
		{
			if (search_path_of_the_name())
			{
				command_invoke();
			}
			else
			{
				find_command_not_found_handle();
			}
		}
	}
	else
	{
		if (search_path_of_the_name())
		{
			command_invoke();
		}
		else
		{
			find_command_not_found_handle();
		}
	}
}

void	do_redirection(t_redir *redir)
{
	t_redir	*cur;
	int		fd;

	cur = redir;
	while (cur != NULL)
	{
		if (redir->type == REDIR_IN)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				perror("open :");
			dup2(redir->filename, stdin);
		}
		else if (redir->type == REDIR_OUT)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				perror("open :");
			dup2(redir->filename, stdout);
		}
		else if (redir->type == REDIR_APPEND)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				perror("open :");
			dup2(redir->filename, stdout);
		}
		else if (redir->type == REDIR_HEREDOC)
		{
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				perror("open :");
			dup2(redir->filename, stdin);
		}
		cur = redir->next;
	}
}
