       #include <stdio.h>
       #include <readline/readline.h>
       #include <readline/history.h>

       char *
       readline (const char *prompt);

int main(void)
{
    char *line;
    line=readline(">");
    return 0;
}
