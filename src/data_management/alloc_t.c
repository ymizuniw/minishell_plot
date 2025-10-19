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

t_redir *alloc_redir(void)
{
    return ((t_redir *)malloc(sizeof(t_redir)));
}
