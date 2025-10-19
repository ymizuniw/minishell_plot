#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_lexer(void)
{
    const char *input = "echo hello world";
    t_token *tokens = lexer(input);
    
    assert(tokens != NULL);
    printf("lexer test passed for input: %s\n", input);
    
    // Clean up tokens
    free_token_list(tokens);
}

int main(void)
{
    test_lexer();
    return 0;
}
