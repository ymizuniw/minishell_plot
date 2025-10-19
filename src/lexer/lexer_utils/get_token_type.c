#include "../../../includes/minishell.h"

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
            else if (input[*idx+1] && input[++*idx]=='>')
                return (TK_REDIRECT_IN_AND_OUT);
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
        else if (input[*idx]=='$')
        {
            if (input[*idx]=='\0' || ((input[*idx+1] && input[*idx+1]=='\"' && input[*idx+2] && input[*idx+2] == '\"') || (input[*idx+1] && input[*idx+1]=='\'' && input[*idx+2] && input[*idx+2] == '\'')))
                return (TK_DOLLER);
        }
        else
            return (TK_WORD);
    }
    else
        return (TK_EOF);
}