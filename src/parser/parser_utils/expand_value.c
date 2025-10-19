#include "../../../includes/minishell.h"

// just expand the value content or apply $ expansion and form a word.
char	*expand_value(t_token *token)
{
	size_t	word_len;
	char	*word;
	char	*tmp_buf;
	char	*doller;
			size_t i;
			char *var;
			char *entry;
				size_t start;

	word_len = strlen(token->value);
	word = NULL;
	tmp_buf = malloc(sizeof(char) * (word_len + 1));
	if (!tmp_buf)
		return (NULL);
	bzero(tmp_buf, sizeof(char) * (word_len + 1));
	if (token->in_dquote || token->in_squote)
	{
		word = malloc(sizeof(char) * (word_len + 1));
		if (!word)
		{
			free(tmp_buf);
			return (NULL);
		}
		strlcpy(word, token->value, word_len + 1);
	}
	else
	{
		doller = strchr(token->value, '$');
		if (doller != NULL)
		{
			i = 0;
			while (doller && doller[i])
			{
				start = i;
				i++;
				while (doller[i] && !isspace(doller[i]) && doller[i] != '$')
					i++;
				var = malloc(sizeof(char) * (i - start));
				if (!var)
				{
					free(tmp_buf);
					free(word);
					return (NULL);
				}
				bzero(var, sizeof(char) * (i - start));
				strlcpy(var, doller + start + 1, i - start);
				entry = getenv(var);
				if (entry != NULL)
					cat_word(&word, entry, strlen(entry));
				free(var);
				if (doller[i] == '$')
					doller = &doller[i];
				else
					doller = NULL;
			}
		}
		else
		{
			word = strdup(token->value);
		}
	}
	free(tmp_buf);
	return (word);
}
