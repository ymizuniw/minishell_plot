#include "minishell.h"

int append_token(t_token *head, t_token *new)
{
    size_t i=0;
    t_token *cur;
    if (head==NULL)
        return (0);
    cur = head->next;
    while (i<head->size)
    {
        cur=cur->next;
    }
    cur->next = new;
    head->size++;
    return (1);
}

//if the token type is TK_WORD, then word_concatenation() concatenate words 
//1. WORD * WORD (tokenization occurs first.)
//2. WORD (cocatenate before tokenization)
      size_t word_concatenation(char **word, size_t word_len, char *input,size_t input_len, size_t idx)
        {
            size_t consumed_len=0;
            //if quote comes
            unsigned char quote_open = is_quote(input[idx]);
            if (quote_open)
            {
                char *quote_close = strchr(&input[idx+1], quote_open);
                if (!quote_close)
                {
                    fprintf(stderr, "syntax error: unclosed quote\n");
                    return NULL;
                }
                //extract word between the quotation.
                size_t new_len = quote_close - &input[idx];
                *word = realloc(word_len + new_len + 1, sizeof(char));
                if (!word)
                    return NULL;
                strlcpy(*word + word_len, &input[idx], new_len + 1);
                consumed_len = word_len + 2;
            }
            //quotation does not come.
            else
            {
                char *tmp_ptr = &input[idx];
                while (idx < input_len && !isspace((unsigned char)input[idx])
                    && is_metachar(input[idx]) == MT_OTHER)
                        tmp_ptr++;
                size_t new_len = tmp_ptr - &input[idx];
                *word = realloc(word_len + 1, sizeof(char));
                if (!*word)
                    return NULL;
                strlcpy(*word + word_len, &input[idx], new_len + 1);
                consumed_len = new_len;
            }
            idx += consumed_len;
            if (idx<input_len && is_meta_char(input[idx])==MT_OTHER)
                idx = word_cocatenation(word, word_len, input, input_len, idx);
            return (idx);
        }

t_token *lexer(const char *input)
{
    size_t idx = 0;
    size_t tmp_idx = 0;
    size_t input_len = strlen(input);
    size_t consumed_idx = 0;
    t_token *token_head;
    char **word=NULL;

    token_head = token_alloc();
    if (token_head==NULL)
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
            continue;
        }
        t_metachar meta = get_meta_char(input[idx]);
        if (meta != MT_OTHER)
        {
            t_token *new = token_alloc();
            new->type = get_token_type((char *)input, &idx);
            set_token_value(new);
            append_token(token_head, new);
            idx++;
            continue;
        }
        else
        {
            size_t word_len=0;
            if (word && *word)
                word_len = strlen(*word);
            idx = word_concatenation(word, word_len, input, input_len, idx);
            t_token *new = token_alloc();
            new->type = TK_WORD;
            size_t word_len = strlen(*word);
            new->value = malloc(sizeof(char)*(word_len+1));
            if (new->value==NULL)
            {
                perror("malloc");
                exit(1);
            }
            new->value[word_len]='\0';
            strlcpy(new->value, *word, word_len + 1);
            new->value = *word;
            new->next=NULL;
            append_token(token_head, new);
        }
    }
}    
