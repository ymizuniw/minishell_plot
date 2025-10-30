#include "includes/minishell.h"
#include <stdio.h>

t_ast *parse(t_token *token_head);

// Mock execution to test traversal order
int exec_command(t_ast *node, t_shell *shell, int execute) {
    t_argv *arg;
    (void)shell;
    (void)execute;
    
    if (!node || !node->cmd) return 0;
    
    printf("EXEC: ");
    arg = (t_argv *)node->cmd->argv;
    while (arg) {
        printf("%s ", arg->word);
        arg = arg->next;
    }
    printf("\n");
    return 0;
}

int exec_pipe(t_ast *node, t_shell *shell, bool execute) {
    (void)shell;
    (void)execute;
    
    printf("PIPE: right=");
    if (node->right && node->right->type == NODE_CMD) {
        t_argv *arg = (t_argv *)node->right->cmd->argv;
        while (arg) {
            printf("%s ", arg->word);
            arg = arg->next;
        }
    }
    printf("| left=");
    if (node->left && node->left->type == NODE_CMD) {
        t_argv *arg = (t_argv *)node->left->cmd->argv;
        while (arg) {
            printf("%s ", arg->word);
            arg = arg->next;
        }
    }
    printf("\n");
    return 0;
}

int exec_subshell(t_ast *node, t_shell *shell, bool execute) {
    (void)node;
    (void)shell;
    (void)execute;
    return 0;
}

int ast_traversal(t_ast *node, t_shell *shell, bool execute);

int main() {
    t_token *head;
    t_ast *root;
    t_shell shell = {0};
    
    printf("=== Test 1: echo a && echo b ===\n");
    printf("Expected order: 'echo a' THEN 'echo b'\n");
    head = lexer("echo a && echo b");
    root = parse(head);
    shell.last_exit_status = 0;
    ast_traversal(root, &shell, true);
    
    printf("\n=== Test 2: a && b && c ===\n");
    printf("Expected order: 'a' THEN 'b' THEN 'c'\n");
    head = lexer("a && b && c");
    root = parse(head);
    shell.last_exit_status = 0;
    ast_traversal(root, &shell, true);
    
    return 0;
}
