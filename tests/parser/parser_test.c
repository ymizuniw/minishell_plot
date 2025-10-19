#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_parser(void)
{
    // Create a simple token list for testing
    t_token *token_head = alloc_token();
    t_token *token1 = alloc_token();
    
    if (token_head && token1) {
        bzero(token_head, sizeof(t_token));
        bzero(token1, sizeof(t_token));
        
        token1->value = strdup("echo");
        append_tokens(token_head, token1);
        
        t_ast *ast = parser(token_head);
        
        printf("parser test completed\n");
        
        if (ast) {
            free_ast_tree(ast);
        }
        free_token_list(token_head);
    }
}

int main(void)
{
    test_parser();
    return 0;
}
