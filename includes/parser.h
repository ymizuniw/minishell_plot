#ifndef PARSER_H
#define PARSER_H

#include "minishell_types.h"

void set_argv(char **argv, t_token *token, size_t i);
t_ast	*gen_command_list(t_ast *ast, t_token *token);

int	check_parenthesis(t_token *token);
int	syntax_check(t_token *token);
t_redir	*redirection(t_redir *redir, t_token *token);
void cat_word(char **word, char *entry, size_t entry_len);
char *expand_value(t_token *token);

#endif