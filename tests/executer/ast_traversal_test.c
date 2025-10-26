#include "../../includes/minishell.h"
#include <assert.h>
#include <stdio.h>

static t_ast *make_simple_cmd_node(void)
{
    t_ast *n = alloc_node();
    bzero(n, sizeof(t_ast));
    n->type = NODE_CMD;
    n->cmd = alloc_cmd();
    bzero(n->cmd, sizeof(t_cmd));
    n->cmd->argv = NULL; // keep NULL so exec_command is a no-op returning 1
    n->cmd->redir = NULL;
    return n;
}

int main(void)
{
    printf("=== ast_traversal test ===\n");
    t_shell shell; bzero(&shell, sizeof(shell));
    shell.interactive = false; shell.last_exit_status = 0; shell.env_list = NULL;

    t_ast *root = make_simple_cmd_node();
    assert(root != NULL);
    int ret = ast_traversal(root, &shell, false);
    assert(ret == 0);
    free_ast_tree(root);
    printf("ok\n");
    return 0;
}
