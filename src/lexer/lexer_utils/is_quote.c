#include "../../../includes/minishell.h"

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
