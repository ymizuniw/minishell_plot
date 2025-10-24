#include "../../../../includes/minishell.h"

#define TEMPLATE  "/tmp/heredoc_tmp_XXXXX"
//generate a random num
unsigned int ft_rand(void)
{
    unsigned int a = 1003;
    unsigned int m = 2147483647;
    unsigned int seed = (a * seed) % m;
    unsigned int random = seed / m;
    return (random);
}

int ft_mkstmpfd(char *template, unsigned int num)
{
    size_t try_limit = 1000;
    if (template==NULL)
        return (-1);
    while (1)
    {
        char *name = ft_mkstmp(template, num);
        int fd = open(name, O_CREAT|O_WRONLY|O_EXCL);
        if (fd>=0)
            return (fd);
        free(name);
        try_limit--;
        if (try_limit==0)
            return -1;
    }
    return (-1);
}

char *ft_mkstmp(char *template, unsigned int num)
{
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char *p;
    size_t len = strlen(template);
    unsigned int num_keep = num;

    char *name  = strdup(template);
    if (!name)
        return (NULL);
    
    p = name + len - 1;
    while (strchr(name, 'X')!=NULL)
    {
        unsigned int random = ft_rand();
        unsigned int idx = num / random % (unsigned int)(sizeof(charset)-1);
        *p = charset[idx];
        num/=10;
        if (num==0)
            num = num_keep;
    }
    return (name);
}
