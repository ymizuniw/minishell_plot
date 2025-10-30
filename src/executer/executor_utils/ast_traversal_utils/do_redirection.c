#include "../../../../includes/minishell.h"

static int	do_each_redirection(char *const *filename, int file_mask,
		int file_perm, int io_num)
{
	int	fd;

	fd = open(filename, file_mask, file_perm);
	if (fd < 0)
		return (-1);
	dup2(fd, io_num);
	close(fd);
}

static int	do_redirect_hd(t_ast *cur)
{
	int	fd;

	fd = make_heredoc(cur);
	if (fd < 0)
		return (-1);
	dup2(fd, STDIN_FILENO);
	close(fd);
}

int	do_redirection(t_ast *node)
{
	t_redir	*cur;
	int		fd;

	if (!node || !node->cmd)
		return (-1);
	cur = node->cmd->redir;
	while (cur != NULL)
	{
		if (cur->type == REDIR_IN)
			do_each_redirection(cur->filename, O_RDONLY, STDIN_FILENO, 0000);
		else if (cur->type == REDIR_HEREDOC)
			do_redirect_hd(cur);
		else if (cur->type == REDIR_OUT)
			do_each_redirection(cur->filename, O_WRONLY | O_CREAT | O_TRUNC,
				0644, STDOUT_FILENO);
		else if (cur->type == REDIR_APPEND)
			do_each_redirection(cur->filename, O_WRONLY | O_CREAT | O_TRUNC,
				0644, STDOUT_FILENO);
		cur = cur->next;
	}
	return (1);
}
