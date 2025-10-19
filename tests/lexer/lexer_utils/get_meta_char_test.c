#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_get_meta_char(void)
{
    t_metachar result;
    
    result = get_meta_char("|");
    printf("get_meta_char test for '|': %d\n", result);
    
    result = get_meta_char("&");
    printf("get_meta_char test for '&': %d\n", result);
    
    result = get_meta_char("a");
    printf("get_meta_char test for 'a': %d\n", result);
    
    printf("get_meta_char tests completed\n");
}

int main(void)
{
    test_get_meta_char();
    return 0;
}
