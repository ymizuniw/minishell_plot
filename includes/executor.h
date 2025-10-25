#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "minishell_types.h"

//child process handler
int handle_child(int *last_exit_status, pid_t pid);

//executor_utils/heredoc
unsigned int ft_rand(unsigned int);
char *ft_mkstmp(char *template);
int ft_mkstmpfd(char *template, char **filename);

//heredoc
int make_heredoc(t_redir *hd);

//executor_utils/search_and_exec
int search_and_exec(char *cmd, t_shell *shell);

//executor_utils/ast_traversal_utils
int	do_redirection(t_ast *node);
int	exec_command(t_ast *node, t_shell *shell, bool execute);
int	exec_pipe(t_ast *node, t_shell *shell, bool execute);
int	exec_subshell(t_ast *node, t_shell *shell, bool execute);
int	ast_traversal(t_ast *node, t_shell *shell, bool execute);

#endif