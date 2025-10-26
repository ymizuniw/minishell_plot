#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_signal_initializer(void)
{
	printf("signal_initializer test\n");
	int ret = signal_initializer(true);
	assert(ret == 0);
	ret = signal_initializer(false);
	assert(ret == 0);
	printf("signal_initializer tests completed\n");
}

int	main(void)
{
	test_signal_initializer();
	printf("ok\n");
	return (0);
}
