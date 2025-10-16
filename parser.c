#include "minishell.h"

t_ast	*parser(t_token *);

t_ast	*parser(t_token *token_head)
{
	t_token	*token;

	token = token_head->next;
	// logic and pipe
	var_expansion();
}

// consider the rule to generation grammar.

// 1. <cmd1> | <cmd2>
// 2. <cmd1> && <cmd2>
// 3. <cmd1> && (cmd2 && cmd3)
// 4. < <cmd1>
// 5. << <cmd1>
// 6. <cmd1> > file_name
// 7. <cmd1> >> file_name

// 1.
//         |
// <cmd1>      <cmd2>

// 2.
//         &&
// <cmd1>      <cmd2>

// 3.
//             &&
// <cmd1>              &&
//             <cmd2>      <cmd3>

// token->type==LPAREN;
// while (token->type!=RPAREN)
//     cur = cur->next;

// t_ast *node;
// node->type = NODE_AND;
// node->right = cmd5_node;
// node->left = cmd_tree;
// cmd_tree =  <cmd1> &&1 (subshell)
// cmd_tree = &&1->left=<cmd1>, &&1->right=<subshell>

// if type==NODE_CMD
// t_cmd cmd = expand_token();
// if type==NODE_*

// 1.logical operator.2.pipe. 3.other.

t_ast	*gen_tree(t_ast *ast, t_token *token)
{
	t_token	*cur_token;
	t_ast	*node;

	cur_token = token;
	if (ast == NULL)
	{
		ast = node_alloc();
		ast->type = expand_token();
		ast->left = ast->right = NULL;
	}
	else if ()
	{
		ast->left = gen_tree(ast->left, token);
	}
	else
		ast->right = gen_tree(ast->right, token);
	return (node);
}

// //後ろから論理演算記号を上位ノードに置く。
// 3.1 <cmd1> &&1 (cmd2 &&2 (cmd3 &&3 cmd4)) &&4 cmd5

//                             &&4
//             &&1                             cmd5
// <cmd1>              $SUBSHELL

// SUBSHELL:
//             &&
// <cmd2>                  &&
//                 <cmd3>      <cmd4>

// 3.2 <cmd1> && <cmd2> && <cmd3>
//                 &&
//     <cmd1>                     &&
//                     <cmd2>              <cmd3>

// 4.
//     < <comd1>

// 5.
//     << <comd1>

// 6.
//     <comd1> > file_name

// 7.
//     <cmd1> >> file_name

// 3.2.4 Lists of Commands

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
		if (token->next->type == TK_WORD)
			return (1);
	}
	if (token_type == TK_HEREDOC)
	{
		if (token->next->type == TK_WORD)
			return (1);
	}
	if (token_type == TK_REDIRECT_OUT)
	{
		if (token->next->type == TK_WORD)
			return (1);
	}
	if (token_type == TK_APPEND)
	{
		if (token->next->type == TK_WORD)
			return (1);
	}
	return (0);
}

t_redir	*redirection(t_ast *node, t_token *token)
{
	t_redir	*cur_redir;
	t_token	*cur_token;

	cur_redir = node->cmd->redir;
	while (cur_redir != NULL)
		cur_redir = cur_redir->next;
	if (token->type == TK_REDIRECT_IN)
	{
		syntax_check(token);
		cur_redir = alloc_redir();
		cur_redir->type = REDIR_IN;
		cur_redir->filename = token->next;
	}
	else if (token->type == TK_HEREDOC)
	{
		syntax_check(token);
		cur_redir = alloc_redir();
		cur_redir->type = REDIR_IN;
		cur_redir->filename = token->next;
	}
	else if (token->type == TK_REDIRECT_OUT)
	{
		syntax_check(token);
		cur_redir = alloc_redir();
		cur_redir->type = REDIR_OUT;
		cur_redir->filename = token->next;
	}
	else if (token->type == TK_APPEND)
	{
		syntax_check(token);
		cur_redir = alloc_redir();
		cur_redir->type = REDIR_APPEND;
		cur_redir->filename = token->next;
	}
	cur_token = token->next->next;
	if (cur_token != NULL && cur_token->type == TK_REDIRECT_IN
		|| cur_token->type == TK_REDIRECT_OUT || cur_token->type == TK_HEREDOC
		|| cur_token->type == TK_APPEND)
		return (redirection(node, cur_token));
	return (node->cmd->redir);
}

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

void expand_token(t_ast *node, t_token *token)
{
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
			if (isspace(token->value[i]) || token->value[i]=='$')
			{
				i++;
				break ;
			}
			i++;
			char var = malloc(sizeof(char)*(i-start));
			if (var==NULL)
			{
				perror("malloc");
				exit(1);
			}
			strlcpy(var, doller+1, i-start);
			char *entry = getenv(var);
			if (entry!=NULL)
				concatenate_word(*word, entry, strlen(entry));
			free(var);
			i++;
		}
		doller = strchr(&token->value[i], '$');
	}
}

t_ast	*gen_command_list(t_ast *ast, t_token *token)
{
	t_ast			*node;
	t_ast			*right_command_list;
	t_ast			*subshell_node;
	t_token			*cur;
	t_token_type	*token_type;
	size_t			consumed_token_count;
	t_redir			*cur_redir;
	t_redir			*redir;

	if (ast == NULL)
	{
		node = alloc_node();
		expand_token(node, token);
		bzero(node, sizeof(t_ast));
	}
	if (token->type == TK_AND_IF)
		node->type = NODE_AND;
	else if (token->type == TK_OR_IF)
		node->type == NODE_OR;
	else if (token->type == TK_RPAREN)
		node->type == NODE_SUBSHELL;
	else if (token->type == TK_WORD)
		node->type == NODE_CMD;
	else if (token->type == TK_PIPE)
		node->type == NODE_PIPE;
	else if (token->type == TK_REDIRECT_IN)
		node->type == NODE_CMD;
	else if (token->type == TK_REDIRECT_OUT)
		node->type == NODE_CMD;
	else if (token->type == TK_HEREDOC)
		node->type == NODE_CMD;
	else if (token->type == TK_APPEND)
		node->type == NODE_CMD;
	else if (token->type == TK_EOF)
		node->type == NODE_CMD;

	
	if (is_operator(node->type))
	{
		swap_and_set_right_node(node, node->parent);
		node->left = gen_command_list(node->left, token->next);
	}
	if (node->type == NODE_SUBSHELL)
	{
		if (!syntax_check(token))
			syntax_err();
		node->subtree = gen_subshell_tree(node->subtree, token,
				&consumed_token_count);
		while (consumed_token_count > 0)
		{
			token = token->next;
			consumed_token_count--;
		}
	}
	else if (node->type == NODE_CMD)
	{
		if (token->type == TK_WORD)
		{
			syntax_check(token);
		}
		redir = redirection(node, token);
	}

}
