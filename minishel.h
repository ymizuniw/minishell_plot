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

//issues
//metachar set and reserved words shall be in separate structures?
//the grammar is determined for only valid patterns? or that syntax will funct if it is implemented?

typedef enum e_metachar
{
    OR,
    AND,
    LPAREN,
    RPAREN,
    LESS,
    GREAT,
    SPACE,
    TAB
} t_metachar;

typedef enum e_reserved
{
    LBRACE,
    RBRACE
} t_reserved;

// Naming:
//         AND_IF '&&' 
//         OR_IF  '||'
//         DLESS  '<<'
//         DGREAT '>>'
//         LESSGREAT '<>'
// RESERVED WORDS
//         Lbrace '{'
//         Rbrace '}'

typedef struct s_token
{
    OR_IF,
    AND_IF,
    LPAREN,
    RPAREN,
    PIPE,
    NEWLINE,
    DLESS,
    DGREAT,
    LESSGREAT
} t_token;

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