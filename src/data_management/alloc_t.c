#include "../includes/minishell.h"

void *alloc_t(int type, size_t n)
{
    if (type==TYPE_AST)
        return ((t_ast *)malloc(sizeof(t_ast)*n));
    if (type==TYPE_TOKEN)
        return ((t_token *)malloc(sizeof(t_token)*n));
    if (type==TYPE_CMD)
        return ((t_cmd *)malloc(sizeof(t_cmd)*n));
    else
        return (NULL);
}

