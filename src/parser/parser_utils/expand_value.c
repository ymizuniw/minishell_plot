#include "../../../includes/minishell.h"

// just expand the value content or apply $ expansion and form a word.
char	*expand_value(t_token *token)
{
	size_t	word_len;
	char	**word;
	char	*tmp_buf;
	char	*doller;
	size_t	i;
	size_t	start;
	char	*var;
	char	*entry;

	word_len = strlen(token->value);
	word = NULL;
	tmp_buf = malloc(sizeof(char) * word_len + 1);
	bzero(tmp_buf, sizeof(char) * (word_len + 1));
	if (token->in_dquote || token->in_squote)
	{
		*word = malloc(sizeof(char) * (word_len + 1));
		bzero(tmp_buf, sizeof(char) * (word_len + 1));
		strlcpy(*word, token->value, word_len);
	}
	else
	{
		doller = strchr(token->value, '$');
		i = 0;
		while (doller != NULL)
		{
			while (word_len > i)
			{
				start = i;
				if (!isspace(doller[i]) || doller[i] != '$')
					i++;
				var = malloc(sizeof(char) * (i - start + 1));
				if (var == NULL)
				{
					perror("malloc");
					exit(1);
				}
				bzero(var, sizeof(char) * (i - start + 1));
				strlcpy(var, doller + 1, i - start);
				entry = getenv(var);
				if (entry != NULL)
					cat_word(word, entry, strlen(entry));
				free(var);
				i++;
			}
			doller = strchr(&doller[i], '$');
		}
	}
	return (*word);
}
