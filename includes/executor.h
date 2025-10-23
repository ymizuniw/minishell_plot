#ifndef EXECUTOR_H
#define EXECUTOR_h

#include "minishell_types.h"

int	ast_traversal(t_ast *node, char **env);

char	*get_unique_filename(void);
int	do_redirection(t_ast *node);

int	exec_command(t_ast *node, char **env);
int	exec_pipe(t_ast *node, char **env);
int	exec_subshell(t_ast *node, char **env);


#endif