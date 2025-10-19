#include "../../../includes/minishell.h"

//it c is meta char, return the specific type, or return other.
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
