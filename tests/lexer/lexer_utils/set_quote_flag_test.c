#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_set_quote_flag(void)
{
    t_token *token = alloc_token();
    char *input = "\"hello world\"";
    char quote_open = '"';
    
    if (token) {
        bzero(token, sizeof(t_token));
        
        set_quote_flag(token, input, quote_open);
        
        printf("set_quote_flag test completed\n");
        
        free(token);
    }
}

int main(void)
{
    test_set_quote_flag();
    return 0;
}
