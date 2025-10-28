#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_prepend_tokens(void)
{
	t_token	*head;
	t_token	*new_token;

	head = alloc_token();
	new_token = alloc_token();
	if (head && new_token)
	{
		bzero(head, sizeof(t_token));
		bzero(new_token, sizeof(t_token));
		new_token->value = strdup("test");
		prepend_tokens(head, new_token);
		assert(head->next == new_token);
		assert(new_token->prev == head);
		printf("prepend_tokens test passed\n");
		free_token_list(head);
	}
}

int	main(void)
{
	test_prepend_tokens();
	return (0);
}
