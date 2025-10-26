#include "../../includes/minishell.h"


/*
name	:is_doller_token
args	:const char *input, size_t *current_idx_p
purpose: find patterns that will be printed as one '$', that are "$", '$', $"var", $'var', $<space>, $<eof>
return : 1 if it is doller token, 0 in other cases.
*/
int is_doller_token(const char *p,  size_t *idx_p)
{
	size_t idx = 0;
	char quote = is_quote(p[idx]);
	if (quote!='\0')
	{
		while (isspace(p[idx]))
			idx++;
		if (p[idx] == '\0' || (p[idx] && p[idx]!='$'))
		{
			*idx_p += idx;
			return (0);
		}
		while (isspace(p[idx]))
			idx++;
		if(quote == p[idx])
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

	/*create dummy head.*/
	idx = 0;
	input_len = strlen(input);
	token_head = alloc_token();
	if (token_head == NULL)
	{
		perror("token_alloc");
		exit(1);
	}
	bzero(token_head, sizeof(t_token));

	/*tokenize input*/

	//loop while input_len reaches null termination.
	while (idx < input_len)
	{
		//skip first spaces.
		if (isspace((unsigned char)input[idx]))
		{
			idx++;
			continue ;
		}
		//allocate memory for new token.
		new = alloc_token();
		//initialize new token.
		bzero(new, sizeof(t_token));
		//get whether it is in meta_char_set or not.
		meta = is_meta_char(input[idx]);
		//if input[idx] is a meta_char, then set the token type to new token, and append it to the head of the token list.
		if (meta != MT_OTHER)
		{
			//get_token_type will get the type of token begin with input[idx], and if it is multiple char token, the idx will consumed internally.
			new->type = get_token_type((char *)input, &idx);
			//append new token to the head of the token list.
			append_tokens(token_head, new);
			//continue the while loop. 
			continue ;
		}
		//if input[idx] is not in meta character set, take into this branch.
		else
		{
			//if input[idx] is doller token
			if (is_doller_token(&input[idx],&idx))
			{
				new->type = TK_DOLLER;
				new->value = strdup("$");
				append_tokens(token_head, new);
				continue ;
			}
			else
			{
				//initialize word.
				word = NULL;
				//word_cat concatenate string part of input starting from idx to word, and return the consumed len of input.
				consumed = word_cat(&word, 0, (char *)input, input_len, idx);
				//if 0 idx is consumed, then word cat failed. report error internally and free memory.
				if (consumed == 0)
				{
					free(new);
					free_token_list(token_head);
					return (NULL);
				}
				//if input[idx] is single or double quote, then set quote flag to new token.
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
	//here means that the input was end and the end of the token list is type of EOF.
	//!? This should be TK_NEWLINE instead of expressing EOF?
	new = alloc_token();
	if (new)
	{
		new->type = TK_EOF;
		new->value = strdup("");
		new->next=NULL;
		append_tokens(token_head, new);
	}
	return (token_head);
}
