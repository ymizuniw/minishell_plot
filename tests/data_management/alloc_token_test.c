#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_alloc_token(void)
{
	t_token	*token;

	token = alloc_token();
	assert(token != NULL);
	printf("alloc_token test passed\n");
	xfree(token);
}

int	main(void)
{
	test_alloc_token();
	return (0);
}
