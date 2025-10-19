#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_set_argv(void)
{
    char **argv = malloc(sizeof(char*) * 3);
    t_token *token = alloc_token();
    size_t i = 0;
    
    if (argv && token) {
        bzero(token, sizeof(t_token));
        token->value = strdup("echo");
        
        set_argv(argv, token, i);
        
        printf("set_argv test completed\n");
        
        if (argv[0]) {
            printf("argv[0]: %s\n", argv[0]);
        }
        
        free_token_list(token);
        free(argv);
    }
}

int main(void)
{
    test_set_argv();
    return 0;
}
