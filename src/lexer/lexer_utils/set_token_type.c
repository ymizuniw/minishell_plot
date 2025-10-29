#include "../../../includes/minishell.h"

// call get_token_type and set token's type.
void	set_token_type(t_token *token, char *input, size_t *tmp_idx)
{
	printf("token: %s\n", token->value);
	printf("type : %d\n", token->type);
	printf("idx  : %zu\n", *tmp_idx);
	token->type = get_token_type(input, tmp_idx);
}
