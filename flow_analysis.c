#include "includes/minishell.h"
#include <stdio.h>

int	main(void)
{
	t_token	*head;
	t_token	*cur;

	printf("======================\n");
	printf("LEXER->PARSER FLOW ISSUE\n");
	printf("======================\n\n");
	printf("For input: 'echo hello | cat'\n\n");
	head = lexer("echo hello | cat");
	printf("Lexer creates (via prepend):\n");
	cur = head;
	while (cur)
	{
		printf("  %s",
			cur->type == TK_HEAD ? "HEAD" : cur->type == TK_EOF ? "EOF" : cur->type == TK_PIPE ? "PIPE" : cur->value ? cur->value : "?");
		if (cur->next)
			printf(" →");
		cur = cur->next;
	}
	printf("\n\n");
	printf("PROBLEM:\n");
	printf("  Parser currently starts from EOF->prev (rightmost)\n");
	printf("  For this input, that's 'echo'\n");
	printf("  But parse_command_list processes 'echo' and 'hello',\n");
	printf("  leaving cur_token at PIPE.\n");
	printf("  Then fgen_tree recurses with PIPE,\n");
	printf("  calling swap_with_parent with NULL parent → CRASH!\n\n");
	printf("ANALYSIS:\n");
	printf("  The parser design is RIGHT-TO-LEFT recursive:\n");
	printf("    1. Build node for current token\n");
	printf("    2. Recurse left\n");
	printf("    3. Attach left child\n\n");
	printf("  For this to work with operators:\n");
	printf("    - Start from RIGHTMOST non-EOF token\n");
	printf("    - If it's an operator, it needs a RIGHT child\n");
	printf("    - But we haven't built anything yet!\n\n");
	printf("ROOT CAUSE:\n");
	printf("  The entry point is WRONG.\n");
	printf("  Should start from LEFTMOST token (HEAD->next),\n");
	printf("  not rightmost (EOF->prev).\n\n");
	printf("  OR: The recursion direction is wrong.\n");
	printf("  Should be left-to-right, not right-to-left.\n\n");
	printf("EXPECTED PARSE TREE for 'echo hello | cat':\n");
	printf("        PIPE\n");
	printf("       /    \\\n");
	printf("     CMD    CMD\n");
	printf("  [echo,    [cat]\n");
	printf("   hello]\n\n");
	printf("With right-to-left recursion:\n");
	printf("  1. Start from 'echo'\n");
	printf("  2. Build CMD[echo, hello]\n");
	printf("  3. Recurse left → PIPE\n");
	printf("  4. PIPE calls swap_with_parent\n");
	printf("  5. Makes CMD the right child\n");
	printf("  6. Recurses left → 'cat'\n");
	printf("  7. Build CMD[cat]\n");
	printf("  8. Attach as left child of PIPE\n");
	printf("  9. ✓ Correct!\n\n");
	printf("So the algorithm CAN work, but...\n");
	printf("The issue is in swap_with_parent:\n");
	printf("  It assumes *parent exists (the right child)\n");
	printf("  But on first call, *parent is NULL!\n\n");
	return (0);
}
