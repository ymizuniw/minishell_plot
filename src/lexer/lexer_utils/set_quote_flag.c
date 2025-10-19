#include "../../../includes/minishell.h"

void set_quote_flag(t_token *token, char *input, char const quote_open)
{
    if (quote_open=='\"')
        token->in_dquote = true;
    else if (quote_open=='\'')
        token->in_squote = true;    
}
