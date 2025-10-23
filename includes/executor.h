#ifndef EXECUTOR_H
#define EXECUTOR_h

#include "minishell_types.h"


//executor_utils/heredoc
int ft_rand(void);
char *ft_mkstmp(char *template, int pid);

//executor_utils/search_and_exec
int search_and_exec(char *cmd, char **env);

//executor_utils/ast_traversal_utils
int	do_redirection(t_ast *node);
int	exec_command(t_ast *node, char **env);
int	exec_pipe(t_ast *node, char **env);
int	exec_subshell(t_ast *node, char **env);

int	ast_traversal(t_ast *node, char **env);

#endif