#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_gen_tree(void)
{
    t_ast *ast = alloc_node();
    t_token *token = alloc_token();
    
    if (ast && token) {
        bzero(ast, sizeof(t_ast));
        bzero(token, sizeof(t_token));
        
        token->value = strdup("echo");
        
        t_ast *result = gen_tree(ast, token);
        
        printf("gen_tree test completed\n");
        
        if (result) {
            free_ast_tree(result);
        }
        free_token_list(token);
    }
}

int main(void)
{
    test_gen_tree();
    return 0;
}
