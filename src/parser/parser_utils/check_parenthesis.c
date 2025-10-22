#include "../../../includes/minishell.h"

// check if parenthesis arrangement is correct.
int	check_parenthesis(t_token **token)
{
	t_token	*cur;

	cur = *token;
	cur = cur->prev;
	while (cur != NULL)
	{
		if (cur->type == TK_LPAREN)
			return (1);
		if (cur->type == TK_RPAREN)
		{
			*token = cur;
			int check = check_parenthesis(token);
			if (check!=1)
				return (-1);
		}
		cur = cur->prev;
	}
	return (-1);
}
