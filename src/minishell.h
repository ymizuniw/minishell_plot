#ifndef MINISHELL_H
# define MINISHELL_H

// # include <readline/history.h>
// # include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define VL_PIPE "|"
# define VL_REDIRECT_IN "<"
# define VL_REDIRECT_OUT ">"
# define VL_HEREDOC "<<"
# define VL_APPEND ">>"
# define VL_EOF "\0"
/*==bonus part==*/
# define VL_AND_IF "&&"
# define VL_OR_IF "||"
# define VL_LPAREN "("
# define VL_RPAREN ")"

typedef enum e_metachar
{
	MT_OTHER,
	MT_OR,
	MT_AND,
	MT_LPAREN,
	MT_RPAREN,
	MT_LESS,
	MT_GREAT,
	MT_SPACE,
	MT_TAB,
	MT_NEWLINE
}					t_metachar;

// token_type classifies which type the token belongs to.
typedef enum e_token_type
{
	TK_WORD,
	TK_PIPE,
	TK_REDIRECT_IN,
	TK_REDIRECT_OUT,
	TK_HEREDOC,
	TK_APPEND,
	TK_EOF,

	/*==bonus part==*/
	TK_AND_IF,
	TK_OR_IF,
	TK_LPAREN,
	TK_RPAREN,
	TK_HEAD
}					t_token_type;

typedef struct s_token
{
	size_t size; // for dummy head to keep the len of the list.
	struct s_token	*prev;
	t_token_type	type;
	bool			in_squote;
	bool			in_dquote;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}					t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}					t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	bool			delim_quoted;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**argv;
	t_redir			*redir;
}					t_cmd;

typedef struct s_ast
{
	struct s_ast	*parent;
	struct s_ast	*subtree;
	struct s_ast	*left;
	t_node_type		type;
	t_cmd			*cmd;
	struct s_ast	*right;
}					t_ast;

typedef struct s_result
{
	t_ast			*root;
	int				exit_code;
}					t_result;

// main.c
t_token				*lexer(const char *input);
t_ast				*parser(t_token *tokens);
t_result			*executor(t_ast *ast);
void				output_manager(t_result *res);

// lexer

t_token_type		get_token_type(char *input, size_t idx);
unsigned char		is_quote(int c);
void				set_token_type(t_token *token, char *input,
						size_t *tmp_idx);
void				set_quote_flag(t_token *token, char *input,
						char const quote_open);
void				set_token_value(t_token *token);

#endif
