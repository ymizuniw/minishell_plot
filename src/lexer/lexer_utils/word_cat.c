#include "../../../includes/minishell.h"

// if the token type is TK_WORD, concatenate it if it is a unit of word,
// in quotation, not delimited by metachar.

int	handle_quotation(char **word, size_t word_len, char const *input,
		size_t *idx, size_t *consumed_len, char quote_open)
{
	size_t	add_len;
	char	*quote_close = NULL;

	if (input[*idx+1])
		quote_close = strchr(&input[*idx + 1], quote_open);
	if (!quote_close)
	{
		// printf("this branch.\n");
		// (*idx)++;
		return (0);
	}
	// printf("add_len: %zu\n", add_len);

	add_len = quote_close - &input[*idx] -1;
	*word = realloc(*word, sizeof(char) * (word_len + add_len + 1));
	if (!*word)
		return (-1);
	strncpy(*word + word_len, &input[*idx+1], add_len);
	(*word)[add_len] = '\0';
	*idx += add_len + 2;
	*consumed_len = add_len;
	return (1);
}

// idx is of input[*idx], and consumed_len is for *word's add_len?
int	handle_plain(char **word, size_t *word_len, char const *input,
		size_t input_len, size_t *idx)
{
	size_t		add_len;
	const char	*tmp_ptr;

	add_len = 0;
	tmp_ptr = &input[*idx];
	while (*idx < input_len && !isspace((int)input[*idx])
		&& is_meta_char(input[*idx]) == MT_OTHER)
		(*idx)++;
	add_len = &input[*idx] - tmp_ptr;
	if (add_len == 0)
		return (-1);
	*word = realloc(*word, sizeof(char) * (*word_len + (add_len) + 1));
	if (!*word)
		return (-1);
	strncpy(*word + *word_len, tmp_ptr, add_len);
	(*word)[*word_len + add_len] = '\0';
	*word_len += add_len;
	return (1);
}

size_t	word_cat(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx)
{
	// printf("\n==word_cat() called.==\ninput[%zu]: %s\n", *idx, &input[*idx]);
	while (*idx<input_len)
	{
		char q_open = is_quote(input[*idx]);
		if (q_open!='\0')
		{
			char *d_close = strchr(&input[*idx + 1], q_open);
			if (d_close)
			{
				size_t ext_len = (size_t)(d_close - &input[*idx + 1]);
				*word = realloc(*word, sizeof(char)*(*idx + ext_len + 2));
				if (*word)
					return 0;
				memcpy(*word + word_len, &input[*idx + 1], ext_len);
				(*word)[word_len + ext_len] = '\0';
				*idx += ext_len + 2;
				word_len += ext_len;
				continue ;
			}
		}
		if (handle_plain(word, &word_len, input, input_len, idx) < 0)
			return (0);
	}
	return (1);
}
