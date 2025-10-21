// command search and expansion
#include "../includes/minishell.h"

// If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.
int exec_plot(t_ast *node, char **env, int last_exit_status)
{
    if (is_operator(node->type))
    {
        if (node->type==NODE_AND)
        {
            //&& creates subshell
            //evaluate previous command's return value,
            //and if value is zero, do the right command,
            //or skip the right command(subshell).
            //redirections are not also done.
            if (last_exit_status==0)
            {
                return (exec_plot(node, env, last_exit_status));
            }
            else
                return (last_exit_status);
        }
        else if (node->type==NODE_OR)
        {
            //&& creates subshell
            //evaluate previous command's return value,
            //and if value is non-zero, do the right command,
            //or skip the right command(subshell).
            //redirections are not also done.
            if (last_exit_status!=0)
            {
                return (exec_plot(node, env, last_exit_status));
            }
            else
                return (last_exit_status);
        }
        else if (node->type==NODE_PIPE)
        {
            //cmd1 | cmd2
            //delegate stdout of cmd1 to cmd2
            //even if cmd1 fails, redirection and cmd2 will be executed.
            //in case of it, the stdin of cmd2 is dev/null.
            //ls [no-arg] : cwd
            //ls [""]   : try to find and err.
            //ls [arg-list-of-filename] : try to find the name and display the name.
            //ls [arg-list-of-dirname]  : try to find the name and display the content.

            //find and create a pipeline first. node shall have a flag int pipeline;

            //if (node->type==NODE_CMD && node->pipeline==true)
            //apply redirection between commands and fork();
            
            //stdin, stdout, stderr.
            //dup2();
            //or operator nodes are skipped?

            //stdout redirection are done, so this node should inherit the types to the next node or leaf.
            close(node->pipeline->out_fd);
            close(stdin);
            dup2(node->pipeline->in_fd, stdin);
            close(node->pipeline->in_fd);
            return (last_exit_status);
        }
        else if (node->type==NODE_SUBSHELL)
        {
            ast_traversal(node->subtree, env);
        }
    }
    do_redirections();
    if (command_name_with_no_slash)
    {
        if (find_the_name())//refer to "shell function"
        {
            command_invoke();
        }
        else
        {
            if (search_built_in_command())
            {
                command_invoke();
            }
            else
            {
                if(search_path_of_the_name())
                {
                    command_invoke();
                }
                else
                {
                    find_command_not_found_handle();
                }
            }
        }
    }
}

int do_pipe_redirection(t_ast *node)
{
    if (node->pipeline->in_pipeline)
    {
        close(node->pipeline->out_fd);
        close(stdin);
        dup2(node->pipeline->in_fd, stdin);
        close(node->pipeline->in_fd);
        dup2(stdout, node->pipeline->out_fd);
        close(stdout);
    }
    else //pipe end.
    {
        close(node->pipeline->out_fd);
        close(stdin);
        dup2(node->pipeline->in_fd, stdin);
        close(node->pipeline->in_fd);
        close(node->pipeline->out_fd);
    }
}

int ast_traversal(t_ast *node, char **env)
{
    t_ast *cur;
    cur=node;
    int last_exit_status=0;

    while (cur!=NULL)
    {
        if (cur->left!=NULL)
            ast_traversal(cur->left, env);
        if (cur->right!=NULL)
            ast_traversal(cur->right, env);
        exec_plot(cur, env, last_exit_status);
    }
    return (last_exit_status);
}
