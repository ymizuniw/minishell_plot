#include "minishell.h"

unsigned char is_quote(int c);
void set_token_type(t_token *token, char *input, size_t *tmp_idx);
void set_quote_flag(t_token *token, char *input, char const quote_open);
void set_token_value(t_token *token, char *input, size_t quote_open_idx,
    size_t quote_close_idx);


unsigned char is_quote(int c)
{
    unsigned char uc = (unsigned char)c;
    if (uc=='\"')
        return ('\"');
    else if (uc=='\'')
        return ('\"');
    else
        return ('\0');
}

t_token_type get_token_type(char *input, size_t idx)
{
    if (input[idx]){
        if (input[idx]=='|')
        {
            //and_if
            if (input[idx+1] && input[idx+1]=='|')
                return (TK_AND_IF);
            else//pipe
                return (TK_PIPE);
        }
        else if (input[idx]=='<')
        {
            if (input[idx+1] && input[idx+1]=='<')
                return (TK_HEREDOC);
            //redirect_in
            else
                return (TK_REDIRECT_IN);
        }
        else if (input[idx]=='>')
        {
            if (input[idx+1] && input[idx+1]=='>')
                return (TK_APPEND);
            //redirect_out
            else
                return (TK_REDIRECT_OUT);
        }
        else if (input[idx]=='&')
        {
            if (input[idx+1] && input[idx+1]=='&')
                return(TK_AND_IF);
            //and_if
        }
        else if (input[idx]=='(')
        {
            return (TK_LPAREN);
        }
        else if (input[idx]==')')
        {
            return (TK_RPAREN);
        }
        else
            return (TK_WORD);
    }
    else
        return (TK_EOF);
}

void set_token_type(t_token *token, char *input, size_t *tmp_idx)
{

    size_t idx = *tmp_idx;

    token->type = get_token_type(input, tmp_idx);

}

void set_quote_flag(t_token *token, char *input, char const quote_open)
{
    if (quote_open=='\"')
        token->in_dquote = true;
    else if (quote_open=='\'')
        token->in_squote = true;    
}

void set_token_value(t_token *token, char *input, size_t idx, size_t quote_open_idx,
    size_t quote_close_idx)
{
    if (token->type==TK_WORD)
    {
        strlcat(token->value, &input[idx], 1);
    }
    else if (token->type==TK_PIPE)
    {
        token->value='|';
    }
    else if (token->type==TK_REDIRECT_IN)
    {
        token->value='<';
        strdup(token->value, input[]);
    }
    else if (token->type==TK_REDIRECT_OUT)
    {
        token->value='>'
    }
    else if (token->type==TK_HEREDOC)
    {
        token->value=
    }
    else if (token->type==TK_APPEND)
    {

    }
    else if (token->type==TK_AND_IF)
    {

    }
    else if (token->type==TK_OR_IF)
    {

    }
    else if (token->type==TK_LPAREN)
    {

    }
    else if (token->type==TK_RPAREN)
    {

    }
    else if (token->type==TK_EOF)
    {

    }
}