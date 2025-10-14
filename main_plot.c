// #include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_token t_token;
typedef struct s_ast t_ast;
typedef struct s_result t_result;

/* 各モジュールの関数プロトタイプ */
t_token    *lexer(const char *input);
t_ast      *parser(t_token *tokens);
t_result   *executor(t_ast *ast);
void        output_manager(t_result *res);

int shell_loop(void)
{
    char     *line;
    t_token  *tokens;
    t_ast    *ast;
    t_result *res;

    while (1)
    {
        line = readline("minishell> ");
        if (!line)
        {
            printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);

        tokens = lexer(line);//formant analisis. 
        ast    = parser(tokens);//abstract structure tree.
        res    = executor(ast);
        output_manager(res);

        free(line);
        free_tokens(tokens);
        free_ast(ast);
        free_result(res);
    }
    return (0);
}
