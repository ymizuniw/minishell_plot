/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_and_exec.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymizuniw <ymizuniw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 17:44:05 by kemotoha          #+#    #+#             */
/*   Updated: 2025/10/31 14:34:39 by ymizuniw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_with_slash(t_shell *shell, char **cmd_args, char **env)
{
	pid_t	pid;

	if (access(cmd_args[0], F_OK) != 0)
	{
		write(2, "bash: ", 6);
		write(2, cmd_args[0], strlen(cmd_args[0]));
		write(2, ": No such file or directory\n", 28);
		shell->last_exit_status = 127;
		return ;
	}
	if (access(cmd_args[0], X_OK) != 0)
	{
		write(2, "bash: ", 6);
		write(2, cmd_args[0], strlen(cmd_args[0]));
		write(2, ": Permission denied\n", 20);
		shell->last_exit_status = 126;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		shell->last_exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		set_sig_dfl();
		execve(cmd_args[0], cmd_args, env);
		perror("execve");
		exit(127);
	}
	handle_child(&shell->last_exit_status, pid);
}

void	search_and_exec(t_shell *shell, char **cmd_args)
{
	int		has_slash;
	char	**env;

	if (!cmd_args || !cmd_args[0])
		return ;
	has_slash = (ft_strchr(cmd_args[0], '/') != NULL);
	if (is_builtin(cmd_args[0]))
	{
		exec_builtin(shell, cmd_args);
		return ;
	}
	env = generate_envp(shell->env_list);
	if (!env)
	{
		shell->last_exit_status = 1;
		return ;
	}
	if (has_slash)
		exec_with_slash(shell, cmd_args, env);
	else
		search_in_path_and_exec(shell, cmd_args);
	free(env);
}
