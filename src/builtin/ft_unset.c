#include "../../includes/minishell.h"

static void remove_env_var(t_env **env_list, const char *key)
{
    t_env *current;
    t_env *prev;

    if (!env_list || !*env_list || !key)
        return;
    current = *env_list;
    prev = NULL;
    while (current)
    {
        if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
        {
            if (!prev)
                *env_list = current->next;
            else
                prev->next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void unset_cmd(t_shell *shell, char **cmd)
{
    int i = 1;
    
    while (cmd[i])
    {
		if (!is_valid_varname(cmd[i]))
        {
            write(2, "unset: `", 8);
            write(2, cmd[i], ft_strlen(cmd[i]));
            write(2, "': not a valid identifier\n", 26);
        }
        else
        	remove_env_var(&shell->env_list, cmd[i]);
        i++;
    }
}
