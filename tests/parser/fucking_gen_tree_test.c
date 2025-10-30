#include "../../includes/data_management.h"
#include "../../includes/minishell_types.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Forward declarations for fucking_gen_tree.c functions
int					token_is_redirection(t_token_type type);
int					parse_redirection(t_redir **redir_head,
						t_token_type token_type, t_token *filename_token);
int					parse_simple_command(t_argv **argv_head,
						t_token *command_token);
int					parse_command_list(t_cmd *cmd, t_token **cur_token);
int					token_is_operator(t_token_type type);
int					token_is_subshell_close(t_token_type type);
int					token_is_command(t_token_type type);
int					token_is_newline_or_eof(t_token_type type);
t_ast				*swap_with_parent(t_ast **parent, t_token **cur_token);
t_ast				*gen_subshell_node(t_ast *parent, t_token **cur_token);
t_ast				*gen_command_node(t_ast *parent, t_token **cur_token);
t_ast				*gen_eof_newline_node(t_ast *parent, t_token **cur_token);
void				fgen_tree(t_ast **parent, t_token **cur_token);
t_ast				*parse(t_token *token_head);

// Helpers to free structures built by the alt parser (argv is a t_argv list)
static void	free_argv_list(t_argv *head)
{
	t_argv	*cur;
	t_argv	*next;

	cur = head;
	while (cur)
	{
		next = cur->next;
		if (cur->word)
			free(cur->word);
		free(cur);
		cur = next;
	}
}

static void	free_ast_shallow_cmd(t_ast *node)
{
	if (!node)
		return ;
	if (node->cmd)
	{
		if (node->cmd->argv)
			free_argv_list((t_argv *)node->cmd->argv);
		free(node->cmd);
	}
	free(node);
}

// Helper: Create a simple token
t_token	*create_token(t_token_type type, const char *value, t_token *prev,
		t_token *next)
{
	t_token	*token;

	token = alloc_token();
	memset(token, 0, sizeof(t_token));
	token->type = type;
	if (value)
		token->value = strdup(value);
	token->prev = prev;
	token->next = next;
	return (token);
}

// Helper: Create token chain (in reverse order for fucking_gen_tree logic)
t_token	*create_token_chain(void)
{
	t_token	*head;

	head = create_token(TK_HEAD, NULL, NULL, NULL);
	return (head);
}

static const char	*node_type_to_str(t_node_type t)
{
	switch (t)
	{
	case NODE_CMD:
		return ("CMD");
	case NODE_PIPE:
		return ("PIPE");
	case NODE_AND:
		return ("AND");
	case NODE_OR:
		return ("OR");
	case NODE_SUBSHELL:
		return ("SUBSHELL");
	default:
		return ("?");
	}
}

// Helper: Print AST structure (detailed)
void	print_ast(t_ast *node, int depth)
{
	int			i;
	t_redir		*r;
	const char	*rt;

	r = alloc_redir();
	rt = r->type == REDIR_IN ? "<" : r->type == REDIR_OUT ? ">" : r->type == REDIR_APPEND ? ">>" : r->type == REDIR_HEREDOC ? "<<" : "?";
	if (!node)
		return ;
	for (i = 0; i < depth; i++)
		printf("  ");
	printf("Node[%p] type: %s (parent=%p)", (void *)node,
		node_type_to_str(node->type), (void *)node->parent);
	if (node->cmd)
	{
		printf(" (CMD)");
		if (node->cmd->argv)
		{
			t_argv *arg = (t_argv *)node->cmd->argv; // Cast for testing
			printf(" argv=");
			while (arg)
			{
				printf("[%s]%s", arg->word, arg->next ? "," : "");
				arg = arg->next;
			}
		}
		if (node->cmd->redir)
		{
			r = node->cmd->redir;
			printf(" redir=");
			while (r)
			{
				printf("(%s %s)%s", rt, r->filename ? r->filename : "",
					r->next ? "," : "");
				r = r->next;
			}
		}
	}
	printf("\n");
	if (node->left)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Left ->\n");
		print_ast(node->left, depth + 1);
	}
	if (node->right)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Right ->\n");
		print_ast(node->right, depth + 1);
	}
	if (node->subtree)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Subtree ->\n");
		print_ast(node->subtree, depth + 1);
	}
}

// Validate parent-child connections recursively; return node count
int	validate_ast_connections(t_ast *node)
{
	int	count;

	count = 0;
	if (!node)
		return (0);
	if (node->left)
	{
		assert(node->left->parent == node);
		count += validate_ast_connections(node->left);
	}
	if (node->right)
	{
		assert(node->right->parent == node);
		count += validate_ast_connections(node->right);
	}
	if (node->subtree)
	{
		assert(node->subtree->parent == node);
		count += validate_ast_connections(node->subtree);
	}
	return (count + 1);
}

// Test A: fgen_tree builds a CMD node and consumes cur_token
void	test_fgen_tree_simple_command_and_consumption(void)
{
	t_token		*head;
	t_token		*echo;
	t_token		*hello;
	t_token		*eof;
	t_ast		*root;
	t_argv		*arg;
	int			nodes;
	const char	*words[4] = {0};
	int			count;

	printf("=== Test A: fgen_tree simple command + consumption ===\n");
	// Build tokens in LEXER ORDER (reversed): HEAD -> hello -> echo -> EOF
	// This matches how the lexer creates tokens (prepends, so last word first)
	head = create_token(TK_HEAD, NULL, NULL, NULL);
	hello = create_token(TK_WORD, "hello", head, NULL);
	head->next = hello;
	echo = create_token(TK_WORD, "echo", hello, NULL);
	hello->next = echo;
	eof = create_token(TK_EOF, NULL, echo, NULL);
	echo->next = eof;
	// Use parse() which reads forward from HEAD->next
	root = parse(head);
	printf("  debug: after parse() root=%p\n", (void *)root);
	// Node built
	assert(root != NULL);
	assert(root->cmd != NULL);
	// argv is stored as t_argv list in this alt path
	// Parser reads forward and prepends, correcting the order
	{
		count = 0;
		arg = (t_argv *)root->cmd->argv;
		printf("  debug: root=%p cmd=%p argv_list=%p\n", (void *)root,
			(void *)root->cmd, (void *)arg);
		while (arg && count < 4)
		{
			words[count++] = arg->word;
			arg = arg->next;
		}
		assert(count == 2);
		// With lexer order + forward reading + prepending = correct order
		assert(words[0] && strcmp(words[0], "echo") == 0);
		assert(words[1] && strcmp(words[1], "hello") == 0);
	}
	// Print AST and validate connections
	printf("-- AST dump --\n");
	print_ast(root, 0);
	nodes = validate_ast_connections(root);
	assert(nodes >= 1);
	// Free allocations to keep test leak-free
	free_ast_shallow_cmd(root);
	free(echo->value);
	free(hello->value);
	free(echo);
	free(hello);
	free(eof);
	free(head);
	printf("✓ CMD node built, argv ok (natural order)\n");
	printf("✓ Test A PASSED\n\n");
}

// Test 1: token_is_redirection
void	test_token_is_redirection(void)
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
void	test_token_is_operator(void)
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
void	test_token_is_command(void)
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
void	test_token_is_newline_or_eof(void)
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
void	test_token_is_subshell_close(void)
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
void	test_parse_redirection(void)
{
	t_redir	*redir_head;
	t_token	filename;
	int		result;
	t_redir	*tmp;

	printf("=== Test 6: parse_redirection ===\n");
	redir_head = NULL;
	memset(&filename, 0, sizeof(t_token));
	filename.value = strdup("input.txt");
	filename.in_squote = false;
	filename.in_dquote = false;
	result = parse_redirection(&redir_head, TK_REDIRECT_IN, &filename);
	assert(result == 1);
	assert(redir_head != NULL);
	assert(redir_head->type == REDIR_IN);
	assert(strcmp(redir_head->filename, "input.txt") == 0);
	printf("✓ REDIR_IN parsed correctly\n");
	free(filename.value);
	// Test HEREDOC with quoted delimiter
	filename.value = strdup("EOF");
	filename.in_dquote = true;
	result = parse_redirection(&redir_head, TK_HEREDOC, &filename);
	assert(result == 1);
	assert(redir_head->next != NULL);
	assert(redir_head->next->type == REDIR_HEREDOC);
	assert(redir_head->next->delim_quoted == true);
	printf("✓ HEREDOC with quoted delimiter parsed\n");
	free(filename.value);
	// Test APPEND
	filename.value = strdup("output.log");
	filename.in_dquote = false;
	result = parse_redirection(&redir_head, TK_APPEND, &filename);
	assert(result == 1);
	printf("✓ APPEND parsed correctly\n");
	printf("✓ Test 6 PASSED\n\n");
	free(filename.value);
	// Cleanup
	while (redir_head)
	{
		tmp = redir_head;
		redir_head = redir_head->next;
		free(tmp->filename);
		free(tmp);
	}
}

// Test 7: parse_simple_command
void	test_parse_simple_command(void)
{
	t_argv	*argv_head;
	t_token	word_token;
	t_token	doller_token;
	t_token	squoted_token;
	int		result;
	t_argv	*tmp;
	int		count;
	t_token	dquoted_token;

	printf("=== Test 7: parse_simple_command ===\n");
	// Test 1: TK_WORD (should expand)
	argv_head = NULL;
	memset(&word_token, 0, sizeof(t_token));
	word_token.type = TK_WORD;
	word_token.value = strdup("hello");
	word_token.in_squote = false;
	word_token.in_dquote = false;
	result = parse_simple_command(&argv_head, &word_token);
	assert(result == 1);
	assert(argv_head != NULL);
	assert(strcmp(argv_head->word, "hello") == 0);
	assert(argv_head->to_expand == true);
	printf("✓ TK_WORD parsed with to_expand=true\n");
	free(word_token.value);
	// Test 2: TK_DOLLER (bare $ - should NOT expand)
	// Note: prepending means new items go to head
	memset(&doller_token, 0, sizeof(t_token));
	doller_token.type = TK_DOLLER;
	doller_token.value = strdup("$");
	doller_token.in_squote = false;
	doller_token.in_dquote = false;
	result = parse_simple_command(&argv_head, &doller_token);
	assert(result == 1);
	// After prepending, $ is now at head, hello is at next
	assert(strcmp(argv_head->word, "$") == 0);
	assert(argv_head->to_expand == false);
	printf("✓ TK_DOLLER parsed with to_expand=false\n");
	free(doller_token.value);
	// Test 3: Single-quoted word (should NOT expand)
	memset(&squoted_token, 0, sizeof(t_token));
	squoted_token.type = TK_WORD;
	squoted_token.value = strdup("$USER");
	squoted_token.in_squote = true;
	squoted_token.in_dquote = false;
	result = parse_simple_command(&argv_head, &squoted_token);
	assert(result == 1);
	count = 0;
	tmp = argv_head;
	while (tmp)
	{
		// After prepending $USER, it's at position 0 (head)
		if (count == 0)
		{
			assert(strcmp(tmp->word, "$USER") == 0);
			assert(tmp->to_expand == false);
		}
		count++;
		tmp = tmp->next;
	}
	assert(count == 3);
	printf("✓ Single-quoted word parsed with to_expand=false\n");
	free(squoted_token.value);
	// Test 4: Double-quoted word (should expand)
	memset(&dquoted_token, 0, sizeof(t_token));
	dquoted_token.type = TK_WORD;
	dquoted_token.value = strdup("$HOME");
	dquoted_token.in_squote = false;
	dquoted_token.in_dquote = true;
	result = parse_simple_command(&argv_head, &dquoted_token);
	assert(result == 1);
	count = 0;
	tmp = argv_head;
	while (tmp)
	{
		// After prepending $HOME, it's at position 0 (head)
		if (count == 0)
		{
			assert(strcmp(tmp->word, "$HOME") == 0);
			assert(tmp->to_expand == true);
		}
		count++;
		tmp = tmp->next;
	}
	assert(count == 4);
	printf("✓ Double-quoted word parsed with to_expand=true\n");
	free(dquoted_token.value);
	// Cleanup
	while (argv_head)
	{
		tmp = argv_head;
		argv_head = argv_head->next;
		free(tmp->word);
		free(tmp);
	}
	printf("✓ Test 7 PASSED\n\n");
}

// Test 8: Integration - Simple command with redirection
void	test_simple_command_with_redirection(void)
{
	t_token	*head;
	t_token	*cat;
	t_token	*redirect;
	t_token	*file;
	t_token	*eof;

	printf("=== Test 8: Simple command with redirection (simulated) ===\n");
	// Create token chain in reverse: EOF <- output.txt <- > <- cat <- HEAD
	head = create_token(TK_HEAD, NULL, NULL, NULL);
	cat = create_token(TK_WORD, "cat", head, NULL);
	head->next = cat;
	redirect = create_token(TK_REDIRECT_OUT, ">", cat, NULL);
	cat->next = redirect;
	file = create_token(TK_WORD, "output.txt", redirect, NULL);
	redirect->next = file;
	eof = create_token(TK_EOF, NULL, file, NULL);
	file->next = eof;
	printf("✓ Token chain created (reverse order)\n");
	printf("  Order: HEAD -> cat -> > -> output.txt -> EOF\n");
	printf("  Note: fucking_gen_tree expects reverse traversal\n\n");
	// Cleanup
	free(cat->value);
	free(file->value);
	free(cat);
	free(redirect->value);
	free(redirect);
	free(file);
	free(eof);
	free(head);
}

// Test 9: swap_with_parent (forward reading: previous becomes LEFT child)
void	test_swap_with_parent(void)
{
	t_ast	*parent;
	t_ast	*new_node;
	t_token	pipe_tok;
	t_token	next_tok;
	t_token	*cur;

	printf("=== Test 9: swap_with_parent ===\n");
	parent = alloc_node();
	memset(parent, 0, sizeof(t_ast));
	parent->type = NODE_CMD;
	// Setup token with next pointer (forward reading needs it)
	memset(&next_tok, 0, sizeof(t_token));
	next_tok.type = TK_WORD;
	memset(&pipe_tok, 0, sizeof(t_token));
	pipe_tok.type = TK_PIPE;
	pipe_tok.next = &next_tok;
	cur = &pipe_tok;
	new_node = swap_with_parent(&parent, &cur);
	assert(new_node != NULL);
	printf("✓ New node created\n");
	assert(new_node->left != NULL);
	printf("✓ Original parent is now left child (forward reading)\n");
	assert(new_node->left->type == NODE_CMD);
	printf("✓ Left child preserved correct type\n");
	assert(new_node->left->parent == new_node);
	printf("✓ Parent pointer updated correctly\n");
	assert(cur == &next_tok);
	printf("✓ cur_token advanced to next token\n");
	printf("✓ Test 9 PASSED\n\n");
	// Cleanup
	free(new_node->left);
	free(new_node);
}

// Test 10: Edge cases
void	test_edge_cases(void)
{
	t_token	*null_token;
	t_ast	*root;
	t_token	*eof;
	t_token	*eof_ptr;
	t_token	*newline;
	t_token	*newline_ptr;

	printf("=== Test 10: Edge Cases ===\n");
	// NULL token
	null_token = NULL;
	root = NULL;
	fgen_tree(&root, &null_token);
	assert(root == NULL);
	printf("✓ NULL token handled gracefully\n");
	// EOF token
	eof = create_token(TK_EOF, NULL, NULL, NULL);
	eof_ptr = eof;
	root = NULL;
	fgen_tree(&root, &eof_ptr);
	assert(root == NULL);
	printf("✓ EOF token returns NULL tree\n");
	free(eof);
	// NEWLINE token
	newline = create_token(TK_NEWLINE, NULL, NULL, NULL);
	newline_ptr = newline;
	root = NULL;
	fgen_tree(&root, &newline_ptr);
	assert(root == NULL);
	printf("✓ NEWLINE token returns NULL tree\n");
	free(newline);
	printf("✓ Test 10 PASSED\n\n");
}

// Test 11: Memory allocation failure handling
void	test_memory_failures(void)
{
	printf("=== Test 11: Memory Failure Handling ===\n");
	printf("⚠️  Manual inspection required\n");
	printf("    - Check alloc_node() NULL returns\n");
	printf("    - Check parse_redirection malloc failures\n");
	printf("    - Verify no memory leaks on error paths\n\n");
}

// Test 12: Reverse order token traversal
void	test_reverse_order_logic(void)
{
	t_token	*head;
	t_token	*echo;
	t_token	*hello;
	t_token	*eof;

	printf("=== Test 12: Reverse Order Token Traversal ===\n");
	// The function expects tokens in reverse order
	// Create: echo <- hello <- EOF
	head = create_token(TK_HEAD, NULL, NULL, NULL);
	echo = create_token(TK_WORD, "echo", head, NULL);
	head->next = echo;
	hello = create_token(TK_WORD, "hello", echo, NULL);
	echo->next = hello;
	eof = create_token(TK_EOF, NULL, hello, NULL);
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

int	main(void)
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
	test_fgen_tree_simple_command_and_consumption();
	test_swap_with_parent();
	test_edge_cases();
	test_memory_failures();
	test_reverse_order_logic();
	printf("================================================\n");
	printf("  SUMMARY\n");
	printf("================================================\n");
	printf("✓ Token type checks: PASSED\n");
	printf("✓ Redirection parsing: PASSED\n");
	printf("✓ Command parsing: PASSED\n");
	printf("✓ Tree manipulation: PASSED\n");
	printf("✓ Edge cases: PASSED\n");
	printf("⚠️  Full gen_tree: NEEDS integration test\n");
	printf("\n");
	printf("CRITICAL NOTES:\n");
	printf("1. Tokens expected in REVERSE order (unusual design)\n");
	printf("2. parse_command_list modifies **cur_token pointer\n");
	printf("3. No error recovery in gen_tree (NULL returns)\n");
	printf("4. syntax_check() is a stub (always returns 1)\n");
	printf("\n");
	return (0);
}
