#include "minishell.h"

t_metachar get_meta_char(char const*c)
{
    if (c=='&')
        return (MT_AND);
    if (c=='|')
        return (MT_OR);
    if (c=='(')
        return (MT_LPAREN);
    if (c==')')
        return (MT_RPAREN);
    if (c=='<')
        return (MT_LESS);
    if (c=='>')
        return (MT_GREAT);
    if (c==' ')
        return (MT_SPACE);
    if (c=='\t')
        return (MT_TAB);
    if (c=='\n')
        return (MT_NEWLINE);
    return (MT_OTHER);
}

unsigned char is_quote(int c)
{
    unsigned char uc = (unsigned char)c;
    if (uc=='\"')
        return ('\"');
    else if (uc=='\'')
        return ('\'');
    else
        return ('\0');
}

//after quotation problems are solved.
t_token_type get_token_type(char *input, size_t *idx)
{
    if (input[*idx]){
        if (input[*idx]=='|')
        {
            if (input[*idx+1] && input[++*idx]=='|')
            {
                return (TK_OR_IF);
            }
            else
                return (TK_PIPE);
        }
        else if (input[*idx]=='<')
        {
            if (input[*idx+1] && input[++*idx]=='<')
                return (TK_HEREDOC);
            else
                return (TK_REDIRECT_IN);
        }
        else if (input[*idx]=='>')
        {
            if (input[*idx+1] && input[++*idx]=='>')
                return (TK_APPEND);
            else
                return (TK_REDIRECT_OUT);
        }
        else if (input[*idx]=='&')
        {
            if (input[*idx+1] && input[++*idx]=='&')
                return(TK_AND_IF);
        }
        else if (input[*idx]=='(')
        {
            return (TK_LPAREN);
        }
        else if (input[*idx]==')')
        {
            return (TK_RPAREN);
        }
        else if (input[*idx]=='$' && input[*idx+1]=='\0')
            return (TK_DOLLER);
        else
            return (TK_WORD);
    }
    else
        return (TK_EOF);
}

void set_token_type(t_token *token, char *input, size_t *tmp_idx)
{
    token->type = get_token_type(input, tmp_idx);
}

void set_quote_flag(t_token *token, char *input, char const quote_open)
{
    if (quote_open=='\"')
        token->in_dquote = true;
    else if (quote_open=='\'')
        token->in_squote = true;    
}

void set_token_value(t_token *token)
{
    if (token->type==TK_PIPE)
        token->value=VL_PIPE;
    else if (token->type==TK_REDIRECT_IN)
        token->value=VL_REDIRECT_IN;
    else if (token->type==TK_REDIRECT_OUT)
        token->value=VL_REDIRECT_OUT;
    else if (token->type==TK_HEREDOC)
        token->value=VL_HEREDOC;
    else if (token->type==TK_APPEND)
        token->value=VL_APPEND;
    else if (token->type==TK_AND_IF)
        token->value=VL_AND_IF;
    else if (token->type==TK_OR_IF)
        token->value=VL_OR_IF;
    else if (token->type==TK_LPAREN)
        token->value=VL_LPAREN;
    else if (token->type==TK_RPAREN)
        token->value=VL_RPAREN;
    else if (token->type==TK_EOF)
        token->value=VL_EOF;
}
