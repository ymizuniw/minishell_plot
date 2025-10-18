#include "../../includes/minishell.h"
// if the token type is TK_WORD, then word_concatenation() concatenate words
// 1. WORD * WORD (tokenization occurs first.)
// 2. WORD (cocatenate before tokenization)
size_t	word_cat(char **word, size_t word_len, char *input,
		size_t input_len, size_t idx)
{
	size_t			consumed_len;
	unsigned char	quote_open;
	char			*quote_close;
	size_t			new_len;
	char			*tmp_ptr;
	size_t			new_len;

	consumed_len = 0;
	// if quote comes
	quote_open = is_quote(input[idx]);
	if (quote_open)
	{
		quote_close = strchr(&input[idx + 1], quote_open);
		if (!quote_close)
		{
			fprintf(stderr, "syntax error: unclosed quote\n");
			return (NULL);
		}
		// extract word between the quotation.
		new_len = quote_close - &input[idx];
		*word = realloc(word_len + new_len + 1, sizeof(char));
		if (!word)
			return (NULL);
		strlcpy(*word + word_len, &input[idx], new_len + 1);
		consumed_len = word_len + 2;
	}
	// quotation does not come.
	else
	{
		tmp_ptr = &input[idx];
		while (idx < input_len && !isspace((unsigned char)input[idx])
			&& is_metachar(input[idx]) == MT_OTHER)
			tmp_ptr++;
		new_len = tmp_ptr - &input[idx];
		*word = realloc(word_len + 1, sizeof(char));
		if (!*word)
			return (NULL);
		strlcpy(*word + word_len, &input[idx], new_len + 1);
		consumed_len = new_len;
	}
	idx += consumed_len;
	if (idx < input_len && is_meta_char(input[idx]) == MT_OTHER)
		idx = word_cat(word, word_len, input, input_len, idx);
	return (idx);
}
