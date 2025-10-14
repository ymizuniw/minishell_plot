#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>

struct s_token t_token;
struct s_ast t_ast;
struct s_result t_result;

// DEFINITIONS
//        The following definitions are used throughout the rest of this document.
//        blank  A space or tab.
//        word   A sequence of characters considered as a single unit by the shell.  Also known as a token.
//        name   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic character or an underscore.  Also
//               referred to as an identifier.
//        metacharacter
//               A character that, when unquoted, separates words.  One of the following:
//               |  & ; ( ) < > space tab
//        control operator
//               A token that performs a control function.  It is one of the following symbols:
//               || & && ; ;; ( ) | <newline>

//metacharacter splits input into tokens, if not quoted.
typedef enum e_metachar
{
    MT_OR,
    MT_AND,
    MT_LPAREN,
    MT_RPAREN,
    MT_LESS,
    MT_GREAT,
    MT_SPACE,
    MT_TAB,
    MT_NEWLINE
} t_metachar;

//token_type classifies which type the token belongs to.
typedef enum e_token_type
{
    TK_WORD,
    TK_PIPE,
    TK_REDIRECT_IN,
    TK_REDIRECT_OUT,
    TK_HEREDOC,
    TK_APPEND,
    TK_EOF

    /*==bonus part==*/
    TK_AND_IF,
    TK_OR_IF,
    TK_LPAREN,
    TK_RPAREN,
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
}

typedef struct s_ast
{

} t_ast;

typedef struct s_result
{

} t_result;

//main.c
t_token    *lexer(const char *input);
t_ast      *parser(t_token *tokens);
t_result   *executor(t_ast *ast);
void        output_manager(t_result *res);

#endif