#include "minishell.h"

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

//check if the token arrangement syntax is correct
int	syntax_check(t_token *token)
{
	t_token_type	token_type;

	token_type = token->type;
	if (token_type == TK_LPAREN)
		return (0);
	if (token_type == TK_RPAREN)
		return (check_parenthesis(token));
	if (token_type == TK_REDIRECT_IN)
	{
        if (token->prev->type == TK_HEAD || (token->prev->prev->type == TK_HEAD))
            return (0);
		if (!(token->prev->type == TK_WORD && token->prev->prev->type == TK_WORD))
            return (0);
	}
	if (token_type == TK_HEREDOC)
	{
		if (token->prev->type != TK_WORD)//if there if no argment to heredocument, it will read until EOF signal sent.
			return (0);
	}
	if (token_type == TK_REDIRECT_OUT)
	{
        if (token->prev->type == TK_HEAD || (token->prev->prev->type == TK_HEAD))
            return (0);
		if (!(token->prev->type == TK_WORD && token->prev->prev->type == TK_WORD))
			return (0);
	}
	if (token_type == TK_APPEND)
	{
		if (token->prev->type == TK_WORD && token->prev->prev->type == TK_WORD)
			return (0);
	}
	return (1);
}

//create a list of redirection
t_redir	*redirection(t_redir *redir, t_token *token)
{
	t_redir	*cur_redir;
	t_token	*cur_token;

	cur_redir = redir;
	while (cur_redir != NULL)
		cur_redir = cur_redir->next;
	cur_redir = alloc_redir();
	bzero(cur_redir, sizeof(t_redir));
	if (token->type==TK_REDIRECT_IN)
	{
        if (token->next==NULL)
            return ;
		char *file_name = expand_value();
		redir->type=REDIR_IN;
		redir->filename=file_name;
	}
	else if (token->type==TK_REDIRECT_OUT)
	{
		char *file_name = expand_value();
		redir->type=REDIR_OUT;
		redir->filename=file_name;
	}
	else if (token->type==TK_HEREDOC)
	{
		char *delim = token->next->value;
		redir->type=REDIR_HEREDOC;
		redir->filename=delim;
		if (token->next->in_dquote || token->next->in_squote)
			redir->delim_quoted = true;
	}
	else if (token->type==TK_APPEND)
	{
		char *file_name = expand_value();
		redir->type=REDIR_APPEND;
		redir->filename=file_name;
	}
	cur_token = token->next->next;
	if (cur_token->type != TK_HEAD && cur_token->type == TK_REDIRECT_IN
		|| cur_token->type == TK_REDIRECT_OUT || cur_token->type == TK_HEREDOC
		|| cur_token->type == TK_APPEND)
		return (redirection(cur_redir, cur_token));
	return (redir);
}

//concatenate word for get_env()'s argment.
void concatenate_word(char **word, char entry, size_t entry_len)
{
	size_t word_len=0;
	if (word && *word)
		word_len = strlen(*word);
	char *new = malloc(sizeof(char)*(word_len + entry_len + 1));
	if (new==NULL)
	{
		perror("malloc");
		exit(1);
	}
	strlcpy(new, *word, word_len);
	strlcpy(new + word_len, entry, entry_len + 1);
	if (*word)
		free(*word);
	*word = new;
}

//expand TK_WORD token's value.
char *expand_value(t_token *token)
{
	//token, in_squote==true,
	//doller, in_squote==false, in_dquote==true.

	size_t word_len = strlen(token->value);
	size_t remain_len=0;
	char *doller = strchr(token->value, '$');
	char **word=NULL;
	char *tmp_buf = malloc(sizeof(char)*word_len);
	bzero(tmp_buf, sizeof(char)*word_len);
	size_t i = 0;
	while (doller!=NULL)
	{
		while (word_len>i)
		{
			size_t start = i;
			if (!isspace(doller[i]) || doller[i]!='$')
				i++;
			char *var = malloc(sizeof(char)*(i-start+1));
			if (var==NULL)
			{
				perror("malloc");
				exit(1);
			}
			bzero(var, sizeof(char)*(i-start+1));
			strlcpy(var, doller+1, i-start);
			char *entry = getenv(var);
			if (entry!=NULL)
				concatenate_word(*word, entry, strlen(entry));
			free(var);
			i++;
		}
		doller = strchr(&doller[i], '$');
	}
	return (*word);
}
