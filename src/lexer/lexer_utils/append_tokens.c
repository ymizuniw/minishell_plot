#include "../../../includes/minishell.h"

//append_token should add front to parse from last token.
void	append_tokens(t_token *head, t_token *new)
{
	new->next = head->next;
	head->next->prev = new;
	head->next = new;
	new->prev = head;
	head->size++;
	return (1);
}

//expected behavor test

int main(void)
{
	t_token *head = alloc_token();
	bzero(head, sizeof(t_token));

	char *buf[12];
	while (scanf("%s", buf)!=EOF)
	{
		t_token *new = alloc_token();
		bzero(new, sizeof(t_token));
		new->value = calloc(12, sizeof(char));
		strlcpy(new->value, buf, 12);
		append_tokens(head, new);
	}
	size_t count=0;
	while(head->next!=NULL)
	{
		count++;
		printf("token[%zu]:", count);
		printf("%s\n", head->next->value);
	}
	return 0;
}
