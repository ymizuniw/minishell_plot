#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Forward declarations for fucking_gen_tree.c functions
int token_is_redirection(t_token_type type);
int parse_redirection(t_redir **redir_head, t_token_type token_type, t_token *filename_token);
int parse_simple_command(t_argv **argv_head, t_token *command_token);
int parse_command_list(t_cmd *cmd, t_token **cur_token);
int token_is_operator(t_token_type type);
int token_is_subshell_close(t_token_type type);
int token_is_command(t_token_type type);
int token_is_newline_or_eof(t_token_type type);
t_ast *swap_with_parent(t_ast **parent);
t_ast *gen_subshell_node(t_ast *parent, t_token **cur_token);
t_ast *gen_command_node(t_ast *parent, t_token **cur_token);
t_ast *gen_eof_newline_node(t_ast *parent, t_token **cur_token);
void gen_tree(t_ast **parent, t_token **cur_token);

// Helper: Create a simple token
t_token *create_token(t_token_type type, const char *value, t_token *prev, t_token *next)
{
	t_token *token = alloc_token();
	memset(token, 0, sizeof(t_token));
	token->type = type;
	if (value)
		token->value = strdup(value);
	token->prev = prev;
	token->next = next;
	return token;
}

// Helper: Create token chain (in reverse order for fucking_gen_tree logic)
t_token *create_token_chain(void)
{
	t_token *head = create_token(TK_HEAD, NULL, NULL, NULL);
	return head;
}

// Helper: Print AST structure
void print_ast(t_ast *node, int depth)
{
	int i;
	if (!node)
		return;
	
	for (i = 0; i < depth; i++)
		printf("  ");
	printf("Node type: %d", node->type);
	
	if (node->cmd)
	{
		printf(" (CMD)");
		if (node->cmd->argv)
		{
			t_argv *arg = (t_argv*)node->cmd->argv; // Cast for testing
			while (arg)
			{
				printf(" [%s]", arg->word);
				arg = arg->next;
			}
		}
	}
	printf("\n");
	
	if (node->left)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Left:\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Right:\n");
		print_ast(node->right, depth + 1);
	}
	if (node->subtree)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Subtree:\n");
		print_ast(node->subtree, depth + 1);
	}
}

// Test 1: token_is_redirection
void test_token_is_redirection(void)
{
	printf("=== Test 1: token_is_redirection ===\n");
	
	assert(token_is_redirection(TK_REDIRECT_IN) == 1);
	printf("✓ TK_REDIRECT_IN recognized\n");
	
	assert(token_is_redirection(TK_REDIRECT_OUT) == 1);
	printf("✓ TK_REDIRECT_OUT recognized\n");
	
	assert(token_is_redirection(TK_HEREDOC) == 1);
	printf("✓ TK_HEREDOC recognized\n");
	
	assert(token_is_redirection(TK_APPEND) == 1);
	printf("✓ TK_APPEND recognized\n");
	
	assert(token_is_redirection(TK_WORD) == 0);
	printf("✓ TK_WORD not a redirection\n");
	
	assert(token_is_redirection(TK_PIPE) == 0);
	printf("✓ TK_PIPE not a redirection\n");
	
	printf("✓ Test 1 PASSED\n\n");
}

// Test 2: token_is_operator
void test_token_is_operator(void)
{
	printf("=== Test 2: token_is_operator ===\n");
	
	assert(token_is_operator(TK_AND_IF) == 1);
	printf("✓ TK_AND_IF is operator\n");
	
	assert(token_is_operator(TK_OR_IF) == 1);
	printf("✓ TK_OR_IF is operator\n");
	
	assert(token_is_operator(TK_PIPE) == 1);
	printf("✓ TK_PIPE is operator\n");
	
	assert(token_is_operator(TK_WORD) == 0);
	printf("✓ TK_WORD not operator\n");
	
	assert(token_is_operator(TK_REDIRECT_IN) == 0);
	printf("✓ TK_REDIRECT_IN not operator\n");
	
	printf("✓ Test 2 PASSED\n\n");
}

// Test 3: token_is_command
void test_token_is_command(void)
{
	printf("=== Test 3: token_is_command ===\n");
	
	assert(token_is_command(TK_WORD) == 1);
	printf("✓ TK_WORD is command\n");
	
	assert(token_is_command(TK_DOLLER) == 1);
	printf("✓ TK_DOLLER is command\n");
	
	assert(token_is_command(TK_PIPE) == 0);
	printf("✓ TK_PIPE not command\n");
	
	assert(token_is_command(TK_REDIRECT_IN) == 0);
	printf("✓ TK_REDIRECT_IN not command\n");
	
	printf("✓ Test 3 PASSED\n\n");
}

// Test 4: token_is_newline_or_eof
void test_token_is_newline_or_eof(void)
{
	printf("=== Test 4: token_is_newline_or_eof ===\n");
	
	assert(token_is_newline_or_eof(TK_NEWLINE) == 1);
	printf("✓ TK_NEWLINE recognized\n");
	
	assert(token_is_newline_or_eof(TK_EOF) == 1);
	printf("✓ TK_EOF recognized\n");
	
	assert(token_is_newline_or_eof(TK_WORD) == 0);
	printf("✓ TK_WORD not newline/eof\n");
	
	printf("✓ Test 4 PASSED\n\n");
}

// Test 5: token_is_subshell_close
void test_token_is_subshell_close(void)
{
	printf("=== Test 5: token_is_subshell_close ===\n");
	
	assert(token_is_subshell_close(TK_LPAREN) == 1);
	printf("✓ TK_LPAREN is subshell close\n");
	
	assert(token_is_subshell_close(TK_RPAREN) == 0);
	printf("✓ TK_RPAREN not subshell close\n");
	
	assert(token_is_subshell_close(TK_WORD) == 0);
	printf("✓ TK_WORD not subshell close\n");
	
	printf("✓ Test 5 PASSED\n\n");
}

// Test 6: parse_redirection
void test_parse_redirection(void)
{
	printf("=== Test 6: parse_redirection ===\n");
	
	t_redir *redir_head = NULL;
	t_token filename;
	memset(&filename, 0, sizeof(t_token));
	filename.value = strdup("input.txt");
	filename.in_squote = false;
	filename.in_dquote = false;
	
	int result = parse_redirection(&redir_head, TK_REDIRECT_IN, &filename);
	assert(result == 1);
	assert(redir_head != NULL);
	assert(redir_head->type == REDIR_IN);
	assert(strcmp(redir_head->filename, "input.txt") == 0);
	printf("✓ REDIR_IN parsed correctly\n");
	
	// Test HEREDOC with quoted delimiter
	filename.value = strdup("EOF");
	filename.in_dquote = true;
	result = parse_redirection(&redir_head, TK_HEREDOC, &filename);
	assert(result == 1);
	assert(redir_head->next != NULL);
	assert(redir_head->next->type == REDIR_HEREDOC);
	assert(redir_head->next->delim_quoted == true);
	printf("✓ HEREDOC with quoted delimiter parsed\n");
	
	// Test APPEND
	filename.value = strdup("output.log");
	filename.in_dquote = false;
	result = parse_redirection(&redir_head, TK_APPEND, &filename);
	assert(result == 1);
	printf("✓ APPEND parsed correctly\n");
	
	printf("✓ Test 6 PASSED\n\n");
	
	// Cleanup
	while (redir_head)
	{
		t_redir *tmp = redir_head;
		redir_head = redir_head->next;
		free(tmp->filename);
		free(tmp);
	}
}

// Test 7: parse_simple_command (requires alloc_argv implementation)
void test_parse_simple_command(void)
{
	printf("=== Test 7: parse_simple_command ===\n");
	printf("⚠️  Skipping - requires alloc_argv() implementation\n");
	printf("    (Would test: TK_WORD, TK_DOLLER, expansion flags)\n\n");
}

// Test 8: Integration - Simple command with redirection
void test_simple_command_with_redirection(void)
{
	printf("=== Test 8: Simple command with redirection (simulated) ===\n");
	
	// Create token chain in reverse: EOF <- output.txt <- > <- cat <- HEAD
	t_token *head = create_token(TK_HEAD, NULL, NULL, NULL);
	t_token *cat = create_token(TK_WORD, "cat", head, NULL);
	head->next = cat;
	t_token *redirect = create_token(TK_REDIRECT_OUT, ">", cat, NULL);
	cat->next = redirect;
	t_token *file = create_token(TK_WORD, "output.txt", redirect, NULL);
	redirect->next = file;
	t_token *eof = create_token(TK_EOF, NULL, file, NULL);
	file->next = eof;
	
	printf("✓ Token chain created (reverse order)\n");
	printf("  Order: HEAD -> cat -> > -> output.txt -> EOF\n");
	printf("  Note: fucking_gen_tree expects reverse traversal\n\n");
	
	// Cleanup
	free(cat->value);
	free(file->value);
	free(cat);
	free(redirect);
	free(file);
	free(eof);
	free(head);
}

// Test 9: swap_with_parent
void test_swap_with_parent(void)
{
	printf("=== Test 9: swap_with_parent ===\n");
	
	t_ast *parent = alloc_node();
	memset(parent, 0, sizeof(t_ast));
	parent->type = NODE_CMD;
	
	t_ast *new_node = swap_with_parent(&parent);
	
	assert(new_node != NULL);
	printf("✓ New node created\n");
	
	assert(new_node->right != NULL);
	printf("✓ Original parent is now right child\n");
	
	assert(new_node->right->type == NODE_CMD);
	printf("✓ Right child preserved correct type\n");
	
	assert(new_node->right->parent == new_node);
	printf("✓ Parent pointer updated correctly\n");
	
	printf("✓ Test 9 PASSED\n\n");
	
	// Cleanup
	free(new_node->right);
	free(new_node);
}

// Test 10: Edge cases
void test_edge_cases(void)
{
	printf("=== Test 10: Edge Cases ===\n");
	
	// NULL token
	t_token *null_token = NULL;
	t_ast *root = NULL;
	gen_tree(&root, &null_token);
	assert(root == NULL);
	printf("✓ NULL token handled gracefully\n");
	
	// EOF token
	t_token *eof = create_token(TK_EOF, NULL, NULL, NULL);
	t_token *eof_ptr = eof;
	root = NULL;
	gen_tree(&root, &eof_ptr);
	assert(root == NULL);
	printf("✓ EOF token returns NULL tree\n");
	free(eof);
	
	// NEWLINE token
	t_token *newline = create_token(TK_NEWLINE, NULL, NULL, NULL);
	t_token *newline_ptr = newline;
	root = NULL;
	gen_tree(&root, &newline_ptr);
	assert(root == NULL);
	printf("✓ NEWLINE token returns NULL tree\n");
	free(newline);
	
	printf("✓ Test 10 PASSED\n\n");
}

// Test 11: Memory allocation failure handling
void test_memory_failures(void)
{
	printf("=== Test 11: Memory Failure Handling ===\n");
	printf("⚠️  Manual inspection required\n");
	printf("    - Check alloc_node() NULL returns\n");
	printf("    - Check parse_redirection malloc failures\n");
	printf("    - Verify no memory leaks on error paths\n\n");
}

// Test 12: Reverse order token traversal
void test_reverse_order_logic(void)
{
	printf("=== Test 12: Reverse Order Token Traversal ===\n");
	
	// The function expects tokens in reverse order
	// Create: echo <- hello <- EOF
	t_token *head = create_token(TK_HEAD, NULL, NULL, NULL);
	t_token *echo = create_token(TK_WORD, "echo", head, NULL);
	head->next = echo;
	t_token *hello = create_token(TK_WORD, "hello", echo, NULL);
	echo->next = hello;
	t_token *eof = create_token(TK_EOF, NULL, hello, NULL);
	hello->next = eof;
	
	printf("✓ Token chain created in reverse order\n");
	printf("  Expected traversal: hello -> echo\n");
	printf("  (Actual parsing would require alloc_argv)\n\n");
	
	// Cleanup
	free(echo->value);
	free(hello->value);
	free(echo);
	free(hello);
	free(eof);
	free(head);
}

int main(void)
{
	printf("================================================\n");
	printf("  COMPREHENSIVE TEST: fucking_gen_tree.c\n");
	printf("================================================\n\n");
	
	test_token_is_redirection();
	test_token_is_operator();
	test_token_is_command();
	test_token_is_newline_or_eof();
	test_token_is_subshell_close();
	test_parse_redirection();
	test_parse_simple_command();
	test_simple_command_with_redirection();
	test_swap_with_parent();
	test_edge_cases();
	test_memory_failures();
	test_reverse_order_logic();
	
	printf("================================================\n");
	printf("  SUMMARY\n");
	printf("================================================\n");
	printf("✓ Token type checks: PASSED\n");
	printf("✓ Redirection parsing: PASSED\n");
	printf("✓ Tree manipulation: PASSED\n");
	printf("✓ Edge cases: PASSED\n");
	printf("⚠️  Command parsing: NEEDS alloc_argv()\n");
	printf("⚠️  Full gen_tree: NEEDS integration test\n");
	printf("\n");
	printf("CRITICAL NOTES:\n");
	printf("1. alloc_argv() function missing - prevents full testing\n");
	printf("2. Tokens expected in REVERSE order (unusual design)\n");
	printf("3. parse_command_list modifies **cur_token pointer\n");
	printf("4. No error recovery in gen_tree (NULL returns)\n");
	printf("5. syntax_check() is a stub (always returns 1)\n");
	printf("\n");
	
	return 0;
}
