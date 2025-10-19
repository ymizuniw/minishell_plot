#include "../../../includes/minishell.h"

// if the token type is TK_WORD and concatenate it if it is a unit of word, in quotation, not delimited by metachar.
size_t	word_cat(char **word, size_t word_len, char *input,
		size_t input_len, size_t idx)
{
	size_t			consumed_len;
	unsigned char	quote_open;
	char			*quote_close;
	size_t			new_len;
	char			*tmp_ptr;

	consumed_len = 0;
	quote_open = is_quote(input[idx]);
	if (quote_open)
	{
		quote_close = strchr(&input[idx + 1], quote_open);
		if (!quote_close)
		{
			fprintf(stderr, "syntax error: unclosed quote\n");
			return (0);
		}
		new_len = quote_close - &input[idx];
		*word = realloc(*word, sizeof(char)*(word_len + new_len + 1));
		if (!word)
			return (0);
		strlcpy(*word + word_len, &input[idx], new_len + 1);
		consumed_len = word_len + 2;
	}
	else
	{
		tmp_ptr = &input[idx];
		while (idx < input_len && !isspace((int)input[idx])
			&& is_meta_char(input[idx]) == MT_OTHER)
			idx++;
		new_len = &input[idx] - tmp_ptr;
		*word = realloc(*word, sizeof(char)*(word_len + new_len + 1));
		if (!*word)
			return (0);
		strlcpy(*word + word_len, tmp_ptr, new_len + 1);
		consumed_len = new_len;
	}
	if (idx < input_len && is_meta_char(input[idx]) == MT_OTHER)
		return word_cat(word, word_len + consumed_len, input, input_len, idx);
	return (consumed_len);
}
