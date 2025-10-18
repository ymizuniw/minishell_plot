#include "../includes/minishell.h"

t_ast *alloc_node(void)
{
    return (t_ast *)malloc(sizeof(t_ast));
}

t_cmd *alloc_cmd(void)
{
    return (t_cmd *)malloc(sizeof(t_ast));
}

t_token *alloc_token(void)
{
    return ((t_token *)malloc(sizeof(t_token)));
}

void *alloc_t(int type, size_t n)
{
        return ((t_ast *)malloc(sizeof(t_ast)*n));
    if (type==TYPE_TOKEN)
        return ((t_token *)malloc(sizeof(t_token)*n));
    if (type==TYPE_CMD)
        return ((t_cmd *)malloc(sizeof(t_cmd)*n));
    else
        return (NULL);
}

