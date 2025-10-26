#include "../../includes/minishell.h"

/*
name	:is_doller_token
args	:const char *input, size_t *current_idx_p
purpose: find patterns that will be printed as one '$', that are "$", '$',
	$"var", $'var', $<space>, $<eof>
return : 1 if it is doller token, 0 in other cases.
*/
int	is_doller_token(const char *p, size_t *idx_p)
{
	size_t	idx;
	char	quote;

	idx = 0;
	quote = is_quote(p[idx]);
	if (quote != '\0')
	{
		while (isspace(p[idx]))
			idx++;
		if (p[idx] == '\0' || (p[idx] && p[idx] != '$'))
		{
			*idx_p += idx;
			return (0);
		}
		while (isspace(p[idx]))
			idx++;
		if (quote == p[idx])
		{
			*idx_p += idx;
			return (0);
		}
		*idx_p += idx;
		return (0);
	}
	return (0);
}

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
			if (is_doller_token(&input[idx], &idx))
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
				if (is_quote(input[idx]))
					set_quote_flag(new, (char *)input, is_quote(input[idx]));
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
		new->value = strdup("");
		new->next = NULL;
		append_tokens(token_head, new);
	}
	return (token_head);
}
