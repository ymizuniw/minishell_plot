#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
	printf("=== exec_pipe smoke test ===\n");
	// tokens: echo hi | echo bye (argv contents not fully executed; just traversal)
	t_token *head = alloc_token(); bzero(head, sizeof(t_token)); head->type = TK_EOF; head->size = 5;
	t_token *w1 = alloc_token(); bzero(w1, sizeof(t_token)); w1->type = TK_WORD; w1->value = strdup("echo");
	t_token *w2 = alloc_token(); bzero(w2, sizeof(t_token)); w2->type = TK_WORD; w2->value = strdup("hi");
	t_token *pipe = alloc_token(); bzero(pipe, sizeof(t_token)); pipe->type = TK_PIPE; pipe->value = strdup("|");
	t_token *w3 = alloc_token(); bzero(w3, sizeof(t_token)); w3->type = TK_WORD; w3->value = strdup("echo");

	head->next = w1; w1->prev = head;
	w1->next = w2; w2->prev = w1;
	w2->next = pipe; pipe->prev = w2;
	pipe->next = w3; w3->prev = pipe;

	t_ast *root = parser(head);
	assert(root != NULL);
	t_shell shell; bzero(&shell, sizeof(shell));
	int ret = ast_traversal(root, &shell, false);
	assert(ret == 0);
	free_token_list(head);
	free_ast_tree(root);
	printf("ok\n");
	return 0;
}
