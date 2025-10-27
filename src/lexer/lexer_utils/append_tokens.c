#include "../../../includes/minishell.h"

//head is a dummy head. tokens will be appended front.
void	append_tokens(t_token *head, t_token *new)
{
	if (!head || !new)
		return ;
	new->next = head->next;
	if (head->next)
		head->next->prev = new;
	head->next = new;
	new->prev = head;
	head->size++;
}
