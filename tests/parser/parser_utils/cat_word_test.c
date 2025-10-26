#include "../../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

void	test_cat_word(void)
{
	char	*word;
	char	entry_buf[8];
	size_t	entry_len;

	word = strdup("hello");
	strcpy(entry_buf, "_world");
	entry_len = strlen(entry_buf);
	cat_word(&word, entry_buf, entry_len);
	printf("cat_word result: %s\n", word);
	assert(strcmp(word, "hello_world") == 0);
	free(word);
}

int	main(void)
{
	test_cat_word();
	printf("ok\n");
	return (0);
}
