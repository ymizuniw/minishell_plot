#include "../../includes/minishell_types.h"

void init_env_from_envp(t_shell *shell, char **envp)
{
    int i = 0;
    
    while (envp[i])
    {
        char *key = extract_key(envp[i]);
        char *value = extract_value(envp[i]);
        set_variable(shell, key, value, 1);
        free(key);
        free(value);
        i++;
    }
}

void free_env_list(t_env *env_list)
{
    t_env *current;
    t_env *next;

    current = env_list;
    while (current)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}
