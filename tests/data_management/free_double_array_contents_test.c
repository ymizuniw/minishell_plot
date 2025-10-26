#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_free_double_array_contents(void)
{
	char	**array;
	char	*content;

	array = xmalloc(sizeof(char *) * 3);
	content = xmalloc(100);
	array[0] = content;
	array[1] = content + 50;
	array[2] = NULL;
	free_double_array_contents(array, content);
	printf("free_double_array_contents test passed\n");
	xfree(array);
}

int	main(void)
{
	test_free_double_array_contents();
	return (0);
}
