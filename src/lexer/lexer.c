#include "../includes/minishell.h"

//append_token should add front to parse from last token.
void	append_tokens(t_token *head, t_token *new)
{
	new->next = head->next;
	head->next->prev = new;
	head->next = new;
	new->prev = head;
	head->size++;
	return (1);
}

t_metachar is_meta_char(int c);
t_token_type get_token_type(t_token *token);
void set_token_value(t_token *new);
void append_tokens(t_token *head, t_token *new);

t_token	*lexer(const char *input)
{
	size_t		idx;
	size_t		tmp_idx;
	size_t		input_len;
	size_t		consumed_idx;
	t_token		*token_head;
	char		**word;
	t_metachar	meta;
	t_token		*new;
	size_t		word_len;
	t_token		*new;
	size_t		word_len;

	idx = 0;
	tmp_idx = 0;
	input_len = strlen(input);
	consumed_idx = 0;
	word = NULL;
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
		new = token_alloc();
		bzero(new, sizeof(t_token));
		meta = get_meta_char(input[idx]);
		if (meta != MT_OTHER)
		{
			new->type = get_token_type(input[idx]);
			set_token_value(new);
			append_token(token_head, new);
			idx++;
			continue ;
		}
		else
		{
			if (is_quote(input[idx]) && (input[idx + 1] && input[idx+1]=='$') && (input[idx+2] && input[idx + 2] && is_quote(input[idx+2])))
			{
				new->type = TK_DOLLER;
				append_token(token_head, new);
			}
			word_len = 0;
			if (word && *word)
				word_len = strlen(*word);
			idx = word_cat(word, word_len, input, input_len, idx);
			new->type = TK_WORD;
			word_len = strlen(*word);
			new->value = malloc(sizeof(char) * (word_len + 1));
			if (new->value == NULL)
			{
				perror("malloc");
				exit(1);
			}
			new->value[word_len] = '\0';
			strlcpy(new->value, *word, word_len + 1);
			new->value = *word;
			new->next = NULL;
			append_token(token_head, new);
		}
	}
}
