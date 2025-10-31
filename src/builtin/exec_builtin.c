#include "../../includes/minishell.h"

void	exec_builtin(t_shell *shell, char **cmd)
{
	int	fd;

	fd = 1;
	if (!cmd || !shell)
		exit(1);
	if (strcmp(cmd[0], "echo") == 0)
		ft_echo(cmd, fd);
	else if (strcmp(cmd[0], "cd") == 0)
		ft_cd(cmd, shell, fd);
	else if (strcmp(cmd[0], "pwd") == 0)
		ft_pwd(shell, fd);
	else if (strcmp(cmd[0], "export") == 0)
		ft_export(shell, cmd, fd);
	else if (strcmp(cmd[0], "unset") == 0)
		ft_unset(shell, cmd);
	else if (strcmp(cmd[0], "env") == 0)
		ft_env(shell, cmd);
	else if (strcmp(cmd[0], "exit") == 0)
		ft_exit(cmd);
}
