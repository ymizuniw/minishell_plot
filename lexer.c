#include "minishell.h"

// Breaks the input into words and operators,
// 	obeying the quoting rules described in Quoting.
// 	These tokens are separated by metacharacters.
// This step performs alias expansion(see Aliases)

t_token	*lexer(const char *);

t_token	*lexer(const char *input)
{
	t_metachar	meta;
	t_token		*token;
	size_t		idx;
	size_t		input_len;
	char		quote_open;
	size_t		quote_open_idx;
	size_t		quote_close_idx;
	size_t		tmp_idx;

	input_len = strlen(input);
	idx = 0;
	while (idx < input_len)
	{
		meta = get_meta_char();
		if (meta != MT_OTHER)
		{
			// here is to split idx.
		}
		else
		{
			quote_open = is_quote();
			if (quote_open != 0)
			{
				quote_open_idx = idx;
				quote_close_idx = 0;
				tmp_idx = idx + 1;
				while (tmp_idx < input_len)
				{
					if (quote_open == is_quote(input[tmp_idx]))
					{
						quote_close_idx = tmp_idx;
						while (tmp_idx < quote_close_idx)
						{
							set_token_type(token, &tmp_idx);
							set_quote_flag(token, quote_open);
							set_token_value(token, input, quote_open_idx,
								quote_close_idx);
						}
					}
					tmp_idx++;
				}
				idx = tmp_idx;
				if (is_quote(input[idx - 1]) == 0)
				{
					quote_err();
				}
			}
			else
			{
				set_token_type(token, &idx);
				set_quote_flag(token);
				set_value(token);
			}
		}
	}
}
