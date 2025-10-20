//command search and expansion
#include "../includes/minishell.h"

// If no command name results, redirections are performed, but do not affect the current shell environment. A redirection error causes the command to exit with a non-zero status.
int exec_plot(t_ast *node)
{

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