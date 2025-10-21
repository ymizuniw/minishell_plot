// command search and expansion
#include "../includes/minishell.h"

// If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.

            //&& creates subshell
            //evaluate previous command's return value,
            //and if value is zero, do the right command,
            //or skip the right command(subshell).
            //redirections are not also done.

            //&& creates subshell
            //evaluate previous command's return value,
            //and if value is non-zero, do the right command,
            //or skip the right command(subshell).
            //redirections are not also done.

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

int exec_plot(t_ast *node, char **env, int last_exit_status)
{
    int exit_status=0;
    if (is_operator(node->type))
    {
        if (node->type==NODE_AND)
        {
            if (last_exit_status==0)
            {

            }
            else
                return (last_exit_status);
        }
        else if (node->type==NODE_OR)
        {
            if (last_exit_status!=0)
            {
                return (exec_plot(node, env, last_exit_status));
            }
            else
                return (last_exit_status);
        }
        else if (node->type==NODE_PIPE)
        {
            close(node->pipeline->out_fd);
            close(stdin);
            dup2(node->pipeline->in_fd, stdin);
            close(node->pipeline->in_fd);
            return (last_exit_status);
        }
        else if (node->type==NODE_SUBSHELL)
        {
            pid_t pid = fork();
            int exit_status = ast_traversal(node->subtree, env);
            if (exit_status==0)
                return (exit_status);
            else
            {
                //non-zero exit status cases.
            }
        }
    }
    //command
    if (node->type==NODE_CMD)
    {
        do_redirection(node);
        do_pipe_redirection(node);
        const char *path = path_validation(node->cmd->argv[0], env);
        exit_status = execve(path, node->cmd->argv[0], env);
    }
    return (exit_status);
}

//if pipe node comes, connect out to right node.
int do_redirection(t_ast *node)
{
    t_redir *cur = node->cmd->redir_in;
    while (cur!=NULL)
    {
        //redir condition branch
        if (cur->type==REDIR_IN)
        {
            int fd;
            fd = open(cur->filename, O_RDONLY);
            if (cur->type==REDIR_IN)
            {
                dup2(fd, stdin);
                close(fd);
            }
        }
        else if (cur->type==REDIR_HEREDOC)
        {
            int fd;
            fd = open(cur->filename, O_WRONLY|O_APPEND);
            if (cur->type==REDIR_IN)
            {
                dup2(fd, stdin);
                close(fd);
            }
        }
        else if (cur->type==REDIR_OUT)
        {
            int fd;
            fd = open(cur->filename, O_RDONLY|O_TRUNC);
            if (cur->type==REDIR_IN)
            {
                dup2(fd, stdout);
                close(fd);
            }
        }
        else if (cur->type==REDIR_APPEND)
        {
            int fd;
            fd = open(cur->filename, O_WRONLY|O_APPEND);
            if (cur->type==REDIR_IN)
            {
                dup2(fd, stdout);
                close(fd);
            }      
        }
        cur=cur->next;
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
    else
    {
        close(node->pipeline->out_fd);
        close(stdin);
        dup2(node->pipeline->in_fd, stdin);
        close(node->pipeline->in_fd);
        close(node->pipeline->out_fd);
    }
}

char *path_validation(const char *cmd_name, char **env)
{
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

int ast_traversal(t_ast *node, char **env)
{
    t_ast *cur;
    cur=node;
    int last_exit_status=0;

    if (cur!=NULL)
    {
        if (cur->left!=NULL)
        {
            pid_t pid = fork();
            last_exit_status = ast_traversal(cur->left, env);
        }
        if (cur->right!=NULL)
        {
            pid_t pid = fork();
            last_exit_status = ast_traversal(cur->left, env);
        }
        if (cur->type==NODE_CMD)
            last_exit_status = exec_plot(cur, env, last_exit_status);
    }
    return (last_exit_status);
}
