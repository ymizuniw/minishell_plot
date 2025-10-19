#include "../../../includes/minishell.h"

//concatenate word after $ expansion.
void cat_word(char **word, char entry, size_t entry_len)
{
	size_t word_len=0;
	if (word && *word)
		word_len = strlen(*word);
	char *new = malloc(sizeof(char)*(word_len + entry_len + 1));
	if (new==NULL)
	{
		perror("malloc");
		exit(1);
	}
	strlcpy(new, *word, word_len);
	strlcpy(new + word_len, entry, entry_len + 1);
	if (*word)
		free(*word);
	*word = new;
}