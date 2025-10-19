#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_gen_command_list(void)
{
    t_ast *ast = alloc_node();
    t_token *token = alloc_token();
    
    if (ast && token) {
        bzero(ast, sizeof(t_ast));
        bzero(token, sizeof(t_token));
        
        token->value = strdup("echo");
        
        t_ast *result = gen_command_list(ast, token);
        
        printf("gen_command_list test completed\n");
        
        if (result) {
            free_ast_tree(result);
        }
        free_token_list(token);
    }
}

int main(void)
{
    test_gen_command_list();
    return 0;
}
