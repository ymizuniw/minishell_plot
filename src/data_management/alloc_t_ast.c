#include "../includes/minishell.h"

#define TYPE_AST 1
#define TYPE_CMD 2
#define TYPE_TOKEN 3
// #define
// #define

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

