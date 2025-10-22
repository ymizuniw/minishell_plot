#include "../../../../includes/minishell.h"

char *get_unique_filename(void)
{
	int prime = 13;
	//take_mod();
	//char *filename = itoa();
	//int open_ret = open(filename, O_WRONLY|O_EXCL);
	//if (open_ret<0)
	//free(filename);
	//continue;
	//else
	//return filename;
}

int	do_redirection(t_ast *node)
{
	t_redir	*cur;
			int fd;

	cur = node->cmd->redir;
	while (cur != NULL)
	{
		if (cur->type == REDIR_IN)
		{
			fd = open(cur->filename, O_RDONLY);//open fail, "dev/null"?
			dup2(fd, stdin);
			close(fd);
		}
		else if (cur->type == REDIR_HEREDOC)
		{
			cur->filename = get_unique_filename();
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
