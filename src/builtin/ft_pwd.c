#include "../../includes/minishell.h"

void pwd_cmd(t_shell *shell, int fd)
{
    char *cwd;
    (void)shell;
    // 常にgetcwd()で現在のディレクトリを取得
    cwd = get_pwd();
    if (!cwd)
    {
        write(2, "pwd: error retrieving current directory: ", 41);
        perror("getcwd");
        return;
    }
    ft_putstr_fd(cwd, fd);
    ft_putchar_fd('\n', fd);
    free(cwd);
}
