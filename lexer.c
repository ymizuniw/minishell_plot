#include "minishell.h"

// Breaks the input into words and operators,
// 	obeying the quoting rules described in Quoting.
// 	These tokens are separated by metacharacters.
// This step performs alias expansion(see Aliases)

t_token	*lexer(const char *);

get_word(char **word, char *input, size_t input_len, size_t *idx)
{
    char quote;
    char *quote_open=NULL;
    char *quote_close=NULL;
    size_t tmp_idx=*idx;
    size_t word_len=strlen(word);
    size_t consumed_idx=0;
    bool word_end=0;

    quote = is_quote(input[*idx]);
    if (quote != 0)
    {
        quote_open = &input[tmp_idx];
        //1."fuck fuck"
        //2."fuck" fuck
        //3.""
        //4."fuck"fuck
        tmp_idx = idx + 1;
        quote_close = strchr(quote_open, quote);
        if (quote_close==NULL)
        {
            //unquoted err.
        }
        if (quote_close[1]=='\0' || get_meta_char(quote_close[1])!=MT_OTHER)
            word_end = 1;
        size_t new_len = quote_close - &input[tmp_idx];
        if (new_len>=2)
        {
            char *word = realloc(word_len + new_len, sizeof(char));
            if (word==NULL)
            {
                //calloc err.
            }
            strlcpy(word, &input[tmp_idx], word_len);
            word[word_len]='\0';
        }
        consumed_idx = word_len+1;
    }
    else
    {
        word_end = true;
        size_t word_len = 0;
        while (tmp_idx < input_len && get_meta_char(input[tmp_idx])!=MT_OTHER)
        {
            tmp_idx++;
            word_len++;
        }
        char *word = calloc(word_len, sizeof(char));
        if (word==NULL)
        {
            //calloc err.
        }
        strlcpy(word, &input[tmp_idx], word_len);
        word[word_len]='\0';
        consumed_idx = word_len-1;
    }
    *idx += consumed_idx;
    if (word_end==false)
        get_word(word, input, input_len, *idx);
}

t_token	*lexer(const char *input)
{
	t_metachar	meta;
	t_token		*token_head;
    t_token     *cur=NULL;
	size_t		idx;
	size_t		input_len;
	char		quote;
	size_t		quote_open_idx;
	size_t		quote_close_idx;
	size_t		tmp_idx;
    size_t consumed_idx;

    token_head = token_alloc();
    bzero(token_head, sizeof(t_token));
    cur=token_head;
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
            char **word=NULL;
            get_word(word, input, input_len, &idx);
            t_token *new_token = gen_token();
            bzero(new_token, sizeof(t_token));
            new_token->type=TK_WORD;
            set_quote_flag(new_token, quote_open);
            new_token->value=*word;
            idx += consumed_idx;
        }
    }
}
// quote removal test case:
