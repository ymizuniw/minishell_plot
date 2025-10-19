#include "../../../includes/minishell.h"

void set_token_type(t_token *token, char *input, size_t *tmp_idx)
{
    token->type = get_token_type(input, tmp_idx);
}
