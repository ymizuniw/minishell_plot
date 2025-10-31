#include "../../../includes/minishell.h"

// if the token type is TK_WORD, concatenate it if it is a unit of word,
// in quotation, not delimited by metachar.

int	handle_quotation(char **word, size_t word_len, char const *input,
		size_t *idx, size_t *consumed_len, char quote_open)
{
	size_t	add_len;
	char	*quote_close;

	quote_close = strchr(&input[*idx + 1], quote_open);
	if (!quote_close)
	{
		// syntax_error.
		return (-1);
	}
	add_len = quote_close - &input[*idx] + 1;
	// printf("add_len: %zu\n", add_len);
	*word = realloc(*word, sizeof(char) * (word_len + add_len + 1));
	if (!*word)
		return (0);
	strncpy(*word + word_len, &input[*idx], add_len);
	(*word)[add_len] = '\0';
	*idx += add_len;
	*consumed_len = add_len;
	return (1);
}

// idx is of input[*idx], and consumed_len is for *word's add_len?
int	handle_plain(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx, size_t *consumed_len)
{
	size_t		add_len;
	const char	*tmp_ptr;

	add_len = 0;
	tmp_ptr = &input[*idx];
	while (*idx < input_len && !isspace((int)input[*idx])
		&& is_meta_char(input[*idx]) == MT_OTHER && !is_quote(input[*idx]))
		(*idx)++; // here idx is proceeded, so consumed_len is no need, is it?
	add_len = &input[*idx] - tmp_ptr;
	if (add_len == 0)
		return (-1);
	*word = realloc(*word, sizeof(char) * (word_len + (add_len) + 1));
	if (!*word)
		return (-1);
	strncpy(*word + word_len, tmp_ptr, add_len);
	(*word)[word_len + add_len] = '\0';
	*consumed_len = add_len; // next increment is done elsewhere?
	return (1);
}

int	cat_after_quotation(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx, size_t *consumed_len)
{
	// size_t additional = 0;
	// additional = word_cat(word, word_len + *consumed_len, input, input_len,
	// 		idx);
	// if (additional == 0)
	// 	return (-1);
	// *consumed_len += additional;
	word_cat(word, word_len + *consumed_len, input, input_len, idx);
	return (1);
}

size_t	word_cat(char **word, size_t word_len, char const *input,
		size_t input_len, size_t *idx)
{
	size_t			consumed_len;
	unsigned char	quote_open;

	consumed_len = 0;
	quote_open = is_quote(input[*idx]);
	if (quote_open)
	{
		if (handle_quotation(word, word_len, input, idx, &consumed_len,
				quote_open) < 0)
			return (0);
	}
	else
	{
		if (handle_plain(word, word_len, input, input_len, idx,
				&consumed_len) < 0)
			return (0);
	}
	if (*idx < input_len && (is_quote(input[*idx])
			|| (is_meta_char(input[*idx]) == MT_OTHER
				&& !isspace((int)input[*idx]))))
	{
		if (cat_after_quotation(word, word_len, input, input_len, idx,
				&consumed_len) < 0)
			return (0);
	}
	return (consumed_len);
}
