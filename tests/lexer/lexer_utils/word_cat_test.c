#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void test_word_cat(void)
{
    char *word = malloc(100);
    strcpy(word, "hello");
    size_t word_len = strlen(word);
    char *input = " world";
    size_t start_idx = 1;
    size_t end_idx = 6;
    
    size_t result = word_cat(&word, word_len, input, start_idx, end_idx);
    
    printf("word_cat test result: %zu\n", result);
    printf("Concatenated word: %s\n", word);
    
    free(word);
}

int main(void)
{
    test_word_cat();
    return 0;
}
