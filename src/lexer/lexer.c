#include "../../includes/minishell.h"

t_token	*lexer(const char *input)
{
	size_t		idx;
	size_t		input_len;
	t_token		*token_head;
	char		*word;
	t_metachar	meta;
	t_token		*new;
	size_t		consumed;

	idx = 0;
	input_len = strlen(input);
	token_head = alloc_token();
	if (token_head == NULL)
	{
		perror("token_alloc");
		exit(1);
	}
	bzero(token_head, sizeof(t_token));
	while (idx < input_len)
	{
		if (isspace((unsigned char)input[idx]))
		{
			idx++;
			continue ;
		}
		new = alloc_token();
		bzero(new, sizeof(t_token));
		meta = is_meta_char(input[idx]);
		if (meta != MT_OTHER)
		{
			new->type = get_token_type((char *)input, &idx);
			append_tokens(token_head, new);
			idx++;
			continue ;
		}
		else
		{
			if (is_quote(input[idx]) && (input[idx + 1] && input[idx
					+ 1] == '$') && (input[idx + 2] && input[idx + 2]
					&& is_quote(input[idx + 2])))
			{
				new->type = TK_DOLLER;
				append_tokens(token_head, new);
				idx++;
				continue ;
			}
			else
			{
				word = NULL;
				consumed = word_cat(&word, 0, (char *)input, input_len, idx);
				if (consumed == 0)
				{
					free(new);
					break ;
				}
				idx += consumed;
				new->type = TK_WORD;
				new->value = word;
				new->next = NULL;
				append_tokens(token_head, new);
			}
		}
	}
	return (token_head);
}
