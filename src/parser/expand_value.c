#include "../../../includes/minishell.h"

int	handle_no_expansion(char **word, char const *value,
		size_t word_len_no_expansion)
{
	*word = xmalloc(sizeof(char) * (word_len_no_expansion + 1));
	if (!*word)
		return (-1);
	strncpy(*word, value, word_len_no_expansion);
	(*word)[word_len_no_expansion] = '\0';
	return (1);
}

int	handle_doller_expansion(char **word, char *doller)
{
	size_t	i;
	char	*var;
	char	*entry;
	size_t	start;
	size_t	var_len;

	i = 0;
	while (doller && doller[i])
	{
		start = i;
		i++;
		while (doller[i] && !isspace(doller[i]) && doller[i] != '$')
			i++;
		var_len = i - start - 1;
		if (var_len == 0)
		{
			cat_word(word, "$", 1);
			if (doller[i] == '$')
				doller = &doller[i];
			else
				doller = NULL;
			continue ;
		}
		var = xmalloc(sizeof(char) * (var_len + 1));
		if (!var)
			return (-1);
		memset(var, 0, sizeof(char) * (var_len + 1));
		strncpy(var, doller + start + 1, var_len);
		entry = getenv(var);
		if (entry != NULL)
			cat_word(word, entry, strlen(entry));
		xfree(var);
		if (doller[i] == '$')
			doller = &doller[i];
		else
			doller = NULL;
	}
	return (1);
}

int	search_doller_and_expand(char **word, char const *value,
		size_t word_len_no_expansion)
{
	char	*doller;
	size_t	prefix_len;

	doller = strchr(value, '$');
	if (doller != NULL)
	{
		prefix_len = doller - value;
		if (prefix_len > 0)
		{
			*word = xmalloc(sizeof(char) * (prefix_len + 1));
			if (!*word)
				return (-1);
			strncpy(*word, value, prefix_len);
			(*word)[prefix_len] = '\0';
		}
		if (handle_doller_expansion(word, doller) < 0)
			return (-1);
	}
	else
	{
		if (handle_no_expansion(word, value, word_len_no_expansion) < 0)
			return (-1);
	}
	return (1);
}

// just expand the value content or apply $ expansion and form a word.
char	*expand_value(t_token *token)
{
	size_t	word_len_no_expansion;
	char	*word;

	// initialize
	word_len_no_expansion = strlen(token->value);
	word = NULL;
	if (token->in_squote || token->type == TK_DOLLER)
	{
		if (handle_no_expansion(&word, token->value, word_len_no_expansion) < 0)
			return (NULL);
	}
	else
	{
		if (search_doller_and_expand(&word, (const char *)(token->value),
				word_len_no_expansion) < 0)
			return (NULL);
	}
	return (word);
}
