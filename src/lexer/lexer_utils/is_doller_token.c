#include "../../../includes/minishell.h"

/*
name	:is_doller_token
args	:const char *input, size_t *current_idx_p
purpose: find patterns that will be printed as one '$', that are "$", '$',
	$"var", $'var', $<space>, $<eof>
return : 1 if it is doller token, 0 in other cases.
*/
int	is_doller_token(const char *p)
{
	if (p[0]!='$')
		return (0);
	if (p[1] == '\0')
		return (1);
	if (isspace((unsigned char)p[1]))
		return (1);
	if (p[1] == '\'' || p[1] == '\"')
		return (1);
	return (0);
}
