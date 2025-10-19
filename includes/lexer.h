#ifndef LXER_H
#define LXER_H


#include "minishell.h"

//lexer utils
t_metachar get_meta_char(char const*c);
unsigned char is_quote(int c);
t_token_type get_token_type(char *input, size_t *idx);
void set_token_type(t_token *token, char *input, size_t *tmp_idx);
void set_quote_flag(t_token *token, char *input, char const quote_open);

t_metachar is_meta_char(int c);
void append_tokens(t_token *head, t_token *new);

size_t	word_cat(char **word, size_t word_len, char *input,
		size_t input_len, size_t idx);

#endif