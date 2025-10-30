
#include "../../includes/minishell.h"

int	before_doller_cat(char **expanded_word, size_t expanded_word_len,
		char *value, size_t value_len)
{
	*expanded_word = realloc(*expanded_word, sizeof(char) * (expanded_word_len
				+ value_len + 1));
	if (*expanded_word == NULL)
		return (-1);
	memcpy(*expanded_word + expanded_word_len, value, value_len);
	(*expanded_word)[expanded_word_len + value_len] = '\0';
	return (1);
}

int	doller_cat(char **doller, char **expanded_word, size_t expanded_word_len)
{
	size_t	value_idx;
	char	*key;
	char	*value;
	size_t	value_len;

	value_idx = 0;
	while ((*doller)[1 + value_idx] != '\0'
		&& (isalnum((unsigned char)(*doller)[1 + value_idx]) || (*doller)[1
			+ value_idx] == '_'))
		value_idx++;
	key = strndup(*doller + 1, value_idx);
	if (!key)
		return (-1);
	value = getenv(key);
	if (value == NULL)
		value = "";
	free(key);
	key = NULL;
	value_idx++;
	if (*expanded_word != NULL)
		expanded_word_len = strlen(*expanded_word);
	if (value != NULL)
		value_len = strlen(value);
	*expanded_word = realloc(*expanded_word, sizeof(char) * (expanded_word_len
				+ value_len) + 1);
	if (*expanded_word == NULL)
	{
		free(key);
		return (-1);
	}
	memcpy(*expanded_word + expanded_word_len, value, value_len);
	(*expanded_word)[expanded_word_len + value_len] = '\0';
	if (key)
		free(key);
	key = NULL;
	*doller = strchr(*doller + value_idx + 1, '$');
	return (1);
}

// this is the key function for word expansion.
char	*expand_word(char *word)
{
	char	*expanded_word;
	char	*doller;
	char	*value;
	size_t	expanded_word_len;
	size_t	value_len;
	char	*previous_doller;
	size_t	word_len;
	size_t	first_idx;

	if (word == NULL)
		return (strdup(""));
	expanded_word_len = 0;
	value_len = 0;
	value = NULL;
	expanded_word = NULL;
	doller = strchr(word, '$');
	if (!doller)
		return (strdup(word));
	previous_doller = word;
	while (doller)
	{
		value_len = (size_t)(doller - previous_doller);
		if (expanded_word)
			expanded_word_len = strlen(expanded_word);
		if (value)
			value_len = strlen(value);
		first_idx = 0;
		if (previous_doller != word)
			first_idx = 1;
		value = strndup(previous_doller + first_idx, value_len);
		if (!value)
		{
			// free_
			return (NULL);
		}
		before_doller_cat(&expanded_word, expanded_word_len, value, value_len);
		doller_cat(&doller, &expanded_word, expanded_word_len);
		previous_doller = doller;
	}
	word_len = strlen(word);
	if (doller == &word[word_len])
		return (expanded_word);
	else
		before_doller_cat(&expanded_word, expanded_word_len, value, value_len);
	return (expanded_word);
}

//"$" is not handling well, but it is recognized as TK_DOLLER in my lexer,
// so it never comes here.

int	main(void)
{
	size_t	i;
	char	*word_list[] = {"$", NULL};
	char	*expanded_word;

	// "no expansion", "$UNIT", , "$JOINEDbb", "term $", "$"
	i = 0;
	for (i = 0; word_list[i] != NULL; i++)
	{
		expanded_word = expand_word(word_list[i]);
		// just for safety
		if (expanded_word == NULL)
			return (1);
		printf("result: %s\n", expanded_word);
		free(expanded_word);
	}
	// for (i=0;i<1;i++)
	// {
	// 	char *expanded_word = expand_word(word_list[i]);
	// 	printf("result: %s\n", expanded_word);
	// 	free(expanded_word);
	// }
	return (0);
}

/*
	gen_argv() runs on argv_list and extract argv.
	the parser is from end to start, but by parse_simple_command(),
	the ptr for command tokens are proceeded till the end of succeeded command token.

	DATA_STRUCTURE:

	char			*word;
	bool			to_expand;
	struct s_argv	*next;

	for each argv_node, if to_expand is true,
		then expand doller and concatenate it to the word.
	The env_list shall be provided later for this function's argment,
		though there is getenv() temporarily.

	The return value is expanded(if doller is found) word list char **argv,
		for executor.
*/

char	**gen_argv(t_argv *argv_list)
{
	t_argv	*cur_argv;
	size_t	argv_idx;
	char	**argv;

	argv_idx = 0;
	argv = NULL;
	if (argv_list == NULL)
		return (NULL);
	// if cur_argv exists,loop continues.
	while (cur_argv != NULL)
	{
		argv = realloc(argv, sizeof(char *) * (argv_idx + 2));
		if (!argv)
		{
			// free_argv();
			return (NULL);
		}
		if (cur_argv->to_expand == true)
			argv[argv_idx] = expand_word(cur_argv->word);
		else
			argv[argv_idx] = strdup(cur_argv->word);
		argv_idx++;
		cur_argv = cur_argv->next;
	}
	argv[argv_idx] = NULL;
	return (argv);
}
