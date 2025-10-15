#include "minishell.h"

// Breaks the input into words and operators,
// 	obeying the quoting rules described in Quoting.
// 	These tokens are separated by metacharacters.
// This step performs alias expansion(see Aliases)

t_token	*lexer(const char *);

t_token	*lexer(const char *input)
{
	t_metachar	meta;
	t_token		*token_head;
    t_token     *cur=NULL;
	size_t		idx;
	size_t		input_len;
	char		quote_open;
	size_t		quote_open_idx;
	size_t		quote_close_idx;
	size_t		tmp_idx;

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
            char *quote_close=NULL;
			quote_open = is_quote();
			if (quote_open != 0)
			{
                //1."fuck fuck"
                //2.""fuck""
                //3.""
				tmp_idx = idx + 1;
                if (input[tmp_idx]==quote_open)
                {
                    //empty string.
                }
                quote_close = strchr(&input[tmp_idx], quote_open);
                if (quote_close==NULL)
                {
                    //unquoted err.
                }
                t_token *new_token = gen_token();
                bzero(new_token, sizeof(t_token));

                new_token->type=TK_WORD;
                set_quote_flag(new_token, quote_open);
                size_t word_len = quote_close - &input[tmp_idx];
                char *word = calloc(word_len, sizeof(char));
                if (word==NULL)
                {
                    //calloc err.
                }
                strlcpy(word, &input[tmp_idx], word_len);
                word[word_len]='\0';
                cur->next=new_token;
                size_t consumed_idx = word_len+1;
			}
            else
            {
                //not quoted word
                size_t word_len = 0;
                while (input[tmp_idx] && is_metachar(input[tmp_idx])!=MT_OTHER)
                {
                    tmp_idx++;
                    word_len++;
                }
                t_token *new_token = token_alloc();
                bzero(new_token,sizeof(t_token));

                new_token->type=TK_WORD;
                char *word = calloc(word_len, sizeof(char));
                if (word==NULL)
                {
                    //calloc err.
                }
                strlcpy(word, &input[tmp_idx], word_len);
                word[word_len]='\0';
                cur->next=new_token;
                size_t consumed_idx = word_len-1;
            }
            idx += consumed_idx;
        }
    }
}
// quote removal test case:
