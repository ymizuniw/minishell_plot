#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_get_token_type(void)
{
    char *input = "echo hello | cat";
    size_t idx = 0;
    t_token_type type;
    
    // Test for word token
    type = get_token_type(input, &idx);
    printf("get_token_type test for 'echo': %d\n", type);
    
    // Test for pipe token
    idx = 11; // position of '|'
    type = get_token_type(input, &idx);
    printf("get_token_type test for '|': %d\n", type);
    
    printf("get_token_type tests completed\n");
}

int main(void)
{
    test_get_token_type();
    return 0;
}
