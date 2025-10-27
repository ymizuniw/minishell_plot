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
	memset(token_head, 0, sizeof(t_token));
	while (idx < input_len)
	{
		if (input[idx]=='\n')
		{
			if (idx>0)
			{
				char prev = input[idx-1];
				if (prev=='|' || prev == '&' || prev == '(')
				{
					idx++;
					continue ;
				}
			}
			new = alloc_token();
			memset(new, 0, sizeof(t_token));
			new->type == TK_NEWLINE;
			new->value = strdup("\n");
			append_tokens(token_head, new);
			idx++;
			continue ;
		}
		if (isspace((unsigned char)input[idx]))
		{
			idx++;
			continue ;
		}
		new = alloc_token();
		memset(new, 0, sizeof(t_token));
		meta = is_meta_char(input[idx]);
		if (meta != MT_OTHER)
		{
			new->type = get_token_type((char *)input, &idx);
			append_tokens(token_head, new);
			continue ;
		}
		else
		{
			if (is_doller_token(&input[idx]))
			{
				new->type = TK_DOLLER;
				new->value = strdup("$");
				append_tokens(token_head, new);
				continue ;
			}
			else
			{
				word = NULL;
				consumed = word_cat(&word, 0, (char *)input, input_len, idx);
				if (consumed == 0)
				{
					xfree(new);
					free_token_list(token_head);
					return (NULL);
				}
				// if (is_quote(input[idx]))
				// 	set_quote_flag(new, (char *)input, is_quote(input[idx]));
				idx += consumed;
				new->type = TK_WORD;
				new->value = word;
				new->next = NULL;
				append_tokens(token_head, new);
			}
		}
	}
	new = alloc_token();
	if (new)
	{
		new->type = TK_EOF;
		if (!new->value)
			new->value = strdup("");
		new->next = NULL;
		append_tokens(token_head, new);
	}
	return (token_head);
}
