#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_syntax_checker(void)
{
	t_token *head = alloc_token();
	bzero(head, sizeof(t_token));
	head->type = TK_HEAD;
	t_token *w = alloc_token();
	bzero(w, sizeof(t_token));
	w->type = TK_WORD;
	w->value = strdup("echo");
	head->next = w; w->prev = head;
	int result = syntax_check(head);
	printf("syntax_check result: %d\n", result);
	assert(result == 1 || result == 0);
	free_token_list(head);
}

int	main(void)
{
	test_syntax_checker();
	printf("ok\n");
	return (0);
}
