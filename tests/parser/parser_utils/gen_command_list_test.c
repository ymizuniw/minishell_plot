#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

static t_token	*make_tokens_simple_echo(void)
{
	t_token	*head;

	head = alloc_token();
	bzero(head, sizeof(t_token));
	head->type = TK_EOF;
	// word token
	t_token	*w = alloc_token();

	bzero(w, sizeof(t_token));
	w->type = TK_WORD;
	w->value = strdup("echo");
	head->next = w;
	w->prev = head;
	return (head);
}

void	test_gen_tree(void)
{
	t_ast	*result;
	t_token	*head;
	t_token	*cur;

	head = make_tokens_simple_echo();
	cur = head->next;
	result = gen_tree(NULL, &cur, 0);
	assert(result != NULL);
	free_ast_tree(result);
	free_token_list(head);
	printf("gen_tree test completed\n");
}

int	main(void)
{
	test_gen_tree();
	printf("ok\n");
	return (0);
}
