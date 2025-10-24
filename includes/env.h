#ifndef ENV_MANAGE_H
#define ENV_MANAGE_H

#include "minishell_types.h"

char	*get_pwd(void);
int		count_token(char **tokens);
int		is_valid_varname(const char *var);
char	*extract_key(const char *str);
char	*extract_value(const char *str);

void	set_variable(t_shell *shell, char *key, char *value, int exported);
t_env	*find_env(t_env *env_list, const char *key);

#endif