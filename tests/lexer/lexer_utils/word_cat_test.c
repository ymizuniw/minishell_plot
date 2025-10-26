#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
	printf("word_cat smoke test via lexer\n");
	const char *input = "ab\"cd\"ef"; // ab"cd"ef
	t_token *t = lexer(input);
	assert(t != NULL);
	// ensure at least one WORD token exists
	int found = 0; t_token *cur = t->next;
	while (cur) { if (cur->type == TK_WORD) { found = 1; break; } cur = cur->next; }
	assert(found == 1);
	free_token_list(t);
	printf("ok\n");
	return 0;
}
