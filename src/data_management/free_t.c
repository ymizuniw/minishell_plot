#include "../includes/minishell.h"

void free_ast_tree(t_ast *p)
{
    t_ast *cur = p;
    if (cur->left!=NULL)
        free_ast_tree(cur->left);
    if (cur->right!=NULL)
        free_ast_tree(cur->right);
    if (p->cmd!=NULL)
        free_cmd_structure(p->cmd);
    if (p->subtree!=NULL)
        free_subshell_tree(p->subtree);
    free(p);
}

void free_subshell_tree(t_ast *p)
{
    free_ast_tree(p);
}

void free_token_list(t_token *p)
{
    if (p->next!=NULL)
        free_token_list(p->next);
    if (p->type!=TK_WORD)
        free(p->value);
    free(p);
}

void free_double_array_contents(char **p, char *p_content)
{
    if (*p!=NULL)
        free_double_array_contents(p, *p++);
    free(*p);
}

void free_double_array(char **p)
{
    if (*p!=NULL)
        free_double_array_contents(p, *p++);
    free(p);
}

void free_redir_list(t_redir *p)
{
    if (p->next!=NULL)
        free_redir_list(p->next);
    if (p->filename!=NULL)
        free(p->filename);
    free(p);
}

void free_cmd_structure(t_cmd *p)
{
    if (p->argv!=NULL)
        free_double_array(p->argv);
    if (p->redir_in!=NULL)
        free_redir_list(p->redir_in);
    if (p->redir_out!=NULL)
        free_redir_list(p->redir_out);
    free(p);
}

//s_pipeline 