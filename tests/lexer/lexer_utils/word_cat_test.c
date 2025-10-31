#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Forward declarations
size_t word_cat(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx);

// Helper to run a single test case
void run_test(const char *test_name, const char *input, const char *expected)
{
	char *word = NULL;
	size_t idx = 0;
	size_t input_len = strlen(input);
	size_t result;

	printf("Test: %s\n", test_name);
	printf("  Input: \"%s\"\n", input);

	result = word_cat(&word, 0, input, input_len, &idx);

	if (result == 0)
	{
		printf("  ✗ FAILED: word_cat returned 0 (error)\n");
		if (word)
			free(word);
		return;
	}

	if (!word)
	{
		printf("  ✗ FAILED: word is NULL\n");
		return;
	}

	printf("  Output: \"%s\"\n", word);
	printf("  Expected: \"%s\"\n", expected);

	if (strcmp(word, expected) == 0)
	{
		printf("  ✓ PASSED\n\n");
	}
	else
	{
		printf("  ✗ FAILED: mismatch\n\n");
	}

	free(word);
}

void test_single_double_quote(void)
{
	run_test("Single double quote", "\"hello\"", "hello");
}

void test_single_single_quote(void)
{
	run_test("Single single quote", "'world'", "world");
}

void test_multiple_double_quotes(void)
{
	run_test("Multiple double quotes", "\"hello\"\"world\"", "helloworld");
}

void test_multiple_single_quotes(void)
{
	run_test("Multiple single quotes", "'hello''world'", "helloworld");
}

void test_mixed_quotes(void)
{
	run_test("Mixed double and single quotes", "\"hello\"'world'", "helloworld");
}

void test_quotes_with_plain(void)
{
	run_test("Quotes with plain text", "hello\"world\"test", "helloworldtest");
}

void test_three_quotes(void)
{
	run_test("Three consecutive quotes", "'one'\"two\"'three'", "onetwothree");
}

void test_empty_quotes(void)
{
	run_test("Empty double quotes", "\"\"", "");
}

void test_empty_single_quotes(void)
{
	run_test("Empty single quotes", "''", "");
}

void test_multiple_empty_quotes(void)
{
	run_test("Multiple empty quotes", "\"\"''\"\"", "");
}

void test_complex_mixed(void)
{
	run_test("Complex mixed", "pre'quote1'mid\"quote2\"post", "prequote1midquote2post");
}

void test_nested_looking(void)
{
	run_test("Quotes inside quotes", "\"it's\"'a'\"test\"", "it'satest");
}

void test_spaces_in_quotes(void)
{
	run_test("Spaces in quotes", "\"hello   world\"", "hello   world");
}

void test_mixed_spaces_quotes(void)
{
	run_test("Mixed spaces and quotes", "'hello world'\"test\"", "hello worldtest");
}

void test_alternating_quotes(void)
{
	run_test("Alternating quotes", "'a'\"b\"'c'\"d\"'e'", "abcde");
}

void test_unclosed_quote(void)
{
	char *word = NULL;
	size_t idx = 0;
	const char *input = "\"unclosed";
	size_t input_len = strlen(input);

	printf("Test: Unclosed quote\n");
	printf("  Input: \"%s\"\n", input);

	word_cat(&word, 0, input, input_len, &idx);

	if (word)
	{
		printf("  Output: \"%s\"\n", word);
		printf("  ✓ Handled unclosed quote (output: %s)\n\n", word);
		free(word);
	}
	else
	{
		printf("  ✓ Handled unclosed quote (NULL output)\n\n");
	}
}

void test_quote_at_end(void)
{
	run_test("Quote at end", "hello'world'", "helloworld");
}

void test_quote_at_start(void)
{
	run_test("Quote at start", "'hello'world", "helloworld");
}

void test_only_quotes(void)
{
	run_test("Only quotes", "\"hello\"'world'\"test\"", "helloworldtest");
}

void test_long_quoted_string(void)
{
	run_test("Long quoted string", 
		"'this is a very long string inside single quotes that should be handled correctly'",
		"this is a very long string inside single quotes that should be handled correctly");
}

int main(void)
{
	printf("========================================\n");
	printf("  WORD_CAT MULTIPLE QUOTATION TESTS\n");
	printf("========================================\n\n");

	test_single_double_quote();
	test_single_single_quote();
	test_multiple_double_quotes();
	test_multiple_single_quotes();
	test_mixed_quotes();
	test_quotes_with_plain();
	test_three_quotes();
	test_empty_quotes();
	test_empty_single_quotes();
	test_multiple_empty_quotes();
	test_complex_mixed();
	test_nested_looking();
	test_spaces_in_quotes();
	test_mixed_spaces_quotes();
	test_alternating_quotes();
	test_unclosed_quote();
	test_quote_at_end();
	test_quote_at_start();
	test_only_quotes();
	test_long_quoted_string();

	printf("========================================\n");
	printf("  ALL TESTS COMPLETED\n");
	printf("========================================\n");

	return 0;
}
