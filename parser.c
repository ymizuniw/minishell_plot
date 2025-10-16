#include "minishell.h"

t_ast	*parser(t_token *);

t_ast	*parser(t_token *token_head)
{
    t_token *token = token_head->next;
    //logic and pipe
    var_expansion();

}

//consider the rule to generation grammar.

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


//if type==NODE_CMD
//t_cmd cmd = expand_token();
//if type==NODE_*

1.logical operator.
2.pipe.
3.other.

t_ast *gen_tree(t_ast *ast, t_token *token)
{
    t_token *cur_token = token;
    t_ast *node;
    if (ast==NULL)
    {
        ast = node_alloc();
        ast->type = expand_token();
        ast->left=ast->right=NULL;
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
