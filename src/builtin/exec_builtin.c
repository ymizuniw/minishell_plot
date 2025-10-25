#include "../../includes/minishell.h"

void run_builtin(t_shell *shell, char **cmd)
{
    int fd = 1; // 暫定
    
    if (!cmd || !shell)
        exit(1);
    if (ft_strncmp(cmd[0], "echo", 5) == 0)
        echo_cmd(cmd);
    else if (ft_strncmp(cmd[0], "cd", 3) == 0)
        cd_cmd(cmd, shell, fd);
    else if (ft_strncmp(cmd[0], "pwd", 4) == 0)
        pwd_cmd(shell, fd);
    else if (ft_strncmp(cmd[0], "export", 7) == 0)
        export_cmd(shell, cmd, fd);
    else if (ft_strncmp(cmd[0], "unset", 6) == 0)
        unset_cmd(shell, cmd);
    else if (ft_strncmp(cmd[0], "env", 4) == 0)
        env_cmd(shell, cmd);
    else if (ft_strncmp(cmd[0], "exit", 5) == 0)
        exit_cmd(cmd);
}
