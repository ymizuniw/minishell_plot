#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
	printf("=== gen_tree smoke test ===\n");
	const char *input = "echo hi && (ls | wc)";
	t_token *toks = lexer(input);
	assert(toks != NULL);
	t_ast *root = parser(&toks);
	assert(root != NULL);
	free_token_list(toks);
	free_ast_tree(root);
	printf("ok\n");
	return 0;
}
