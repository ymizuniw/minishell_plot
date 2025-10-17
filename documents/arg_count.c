#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	char	*args[] = {"echo", NULL, NULL};

	if (argc < 2)
		return (1);
	for (size_t i = 1; argv[i] != NULL; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			args[1] = argv[i];
			args[2] = NULL;
			execve("/bin/echo", args, envp);
			perror("execve");
		}
		waitpid(pid, NULL, 0);
	}
	return (0);
}

// metachar が来ず、quotation と文字列が連続している場合、それらは無視される。squote, dquote両方で同じ。
// quotation の後に区切り文字が来るかどうか。
// ./a.out "aa"' 'a | cat -e
// aa a$
//  ./a.out "aa"b'c''d' | cat -e
// aabcd$
// double quoteの後にsingle quoteが連続して来た場合、それは展開される。
// double quote の中に来る＄変数とdouble quotation以外の値は展開されない。
// in double quote, single quotes are delt as part of word.

// ymizuniw@companion:~/minishell_plot$ ./a.out " "aa "bb"" "
//  aa
// bb
// ymizuniw@companion:~/minishell_plot$ ./a.out " "aa" bb"" "
//  aa bb
// ymizuniw@companion:~/minishell_plot$ ./a.out " "aa"bb"" "
//  aabb
// ymizuniw@companion:~/minishell_plot$ ./a.out " "aa "bb"" "
//  aa
// bb
// ymizuniw@companion:~/minishell_plot$ ./a.out " "aa "bb"" " | cat -e
//  aa$
// bb $
// ymizuniw@companion:~/minishell_plot$ ./a.out ""aa "bb"" " | cat -e
// aa$
// bb $
// ymizuniw@companion:~/minishell_plot$ ./a.out ""aa"bb"" " | cat -e
// aabb $
// ymizuniw@companion:~/minishell_plot$ ./a.out ""aa "bb"" " | cat -e
// aa$
// bb $
// ymizuniw@companion:~/minishell_plot$ ./a.out aa "bb"" " | cat -e
// aa$
// bb $
// ymizuniw@companion:~/minishell_plot$ ./a.out aa "bb" " " | cat -e
// aa$
// bb$
//  $
// ymizuniw@companion:~/minishell_plot$ ./a.out aa "bb"" " | cat -e
// aa$
// bb $
// ymizuniw@companion:~/minishell_plot$ ./a.out aa "bb"" " | cat -e