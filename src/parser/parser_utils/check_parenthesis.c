#include "../../../includes/minishell.h"

//check if parenthesis arrangement is correct
int	check_parenthesis(t_token *token)
{
	t_token	*cur;

	cur = token;
	cur = cur->prev;
	while (cur != NULL)
	{
		if (cur->type == TK_LPAREN)
			return (1);
		if (cur->type == TK_RPAREN)
			return (check_parenthesis(cur));
		cur = cur->prev;
	}
	return (0);
}
