#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_is_meta_char(void)
{
	t_metachar	result;

	result = is_meta_char('|');
	printf("is_meta_char('|') => %d\n", result);
	assert(result == MT_OR);
	result = is_meta_char('&');
	printf("is_meta_char('&') => %d\n", result);
	assert(result == MT_AND);
	result = is_meta_char('a');
	printf("is_meta_char('a') => %d\n", result);
	assert(result == MT_OTHER);
}

int	main(void)
{
	test_is_meta_char();
	printf("ok\n");
	return (0);
}
