#include "minishell.h"

t_ast	*parser(t_token *);

t_ast	*parser(t_token *token_head)
{
    t_token *token = token_head->next;
    //logic and pipe
    
}

//consider the rule to generation grammar.

1. <cmd1> | <cmd2>
2. <cmd1> && <cmd2>
3. <cmd1> && (cmd2 && cmd3)
4. < <cmd1> 
5. << <cmd1>
6. <cmd1> > file_name
7. <cmd1> >> file_name
