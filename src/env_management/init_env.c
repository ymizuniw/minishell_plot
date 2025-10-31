#include "../../includes/minishell.h"

void	init_env_from_envp(t_shell *shell, char **envp)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		key = extract_key(envp[i]);
		if (key == NULL)
		{
			i++;
			continue ;
		}
		value = extract_value(envp[i]);
		if (value == NULL)
		{
			value = strdup("");
			if (!value)
			{
				xfree(key);
				i++;
				continue ;
			}
		}
		set_variable(shell, key, value, 1);
		xfree(key);
		xfree(value);
		i++;
	}
}

void	free_env_list(t_env *env_list)
{
	if (env_list == NULL)
		return ;
	free_env_list(env_list->next);
	xfree(env_list->key);
	xfree(env_list->value);
	xfree(env_list);
}
