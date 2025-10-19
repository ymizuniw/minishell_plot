#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_cat_word(void)
{
    char *word = malloc(100);
    strcpy(word, "hello");
    char entry = 'x';
    size_t entry_len = 5;
    
    cat_word(&word, entry, entry_len);
    
    printf("cat_word test completed\n");
    printf("Result word: %s\n", word);
    
    free(word);
}

int main(void)
{
    test_cat_word();
    return 0;
}
