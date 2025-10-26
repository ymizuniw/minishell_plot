#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
	printf("=== ft_mkstmp/ft_mkstmpfd test ===\n");
	char *name = ft_mkstmp(HERE_TEMPLATE);
	assert(name != NULL);
	printf("tmp name: %s\n", name);
	xfree(name);

	char *fname = NULL;
	int fd = ft_mkstmpfd(HERE_TEMPLATE, &fname);
	assert(fd >= 0);
	assert(fname != NULL);
	printf("tmp file: %s (fd=%d)\n", fname, fd);
	close(fd);
	unlink(fname);
	xfree(fname);
	printf("ok\n");
	return 0;
}
