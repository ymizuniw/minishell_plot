#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_set_argv(void)
{
	char	**argv;
	t_token	*head;
	t_token	*cur;
	size_t	i;

	argv = calloc(4, sizeof(char*));
	head = alloc_token();
	bzero(head, sizeof(t_token));
	head->type = TK_HEAD;
	cur = alloc_token();
	bzero(cur, sizeof(t_token));
	cur->type = TK_WORD;
	cur->value = strdup("echo");
	head->next = cur; cur->prev = head;
	i = 0;
	set_argv(argv, &cur, i);
	printf("argv[0]=%s\n", argv[0] ? argv[0] : "(null)");
	assert(argv[0] != NULL);
	free_token_list(head);
	for (size_t k=0;k<4;k++) free(argv[k]);
	free(argv);
}

int	main(void)
{
	test_set_argv();
	printf("ok\n");
	return (0);
}
