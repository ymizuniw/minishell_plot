#include "../../../../includes/minishell.h"

//generate a random num
unsigned int ft_rand(unsigned int a)
{
    // unsigned int a = 1003;
    unsigned int m = 2147483647;
    unsigned int seed = (a * seed) % m;
    unsigned int random = seed / m;
    return (random);
}

char *ft_mkstmp(char *template)
{
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWYZabcdefghijklmnopqrstuvwxyz0123456789";
    char *p;
    size_t len = strlen(template);
    unsigned int num;
    void *ptr = &num;
    num = (unsigned long)ptr;
    unsigned int num_keep = num;
    char *name  = strdup(template);
    if (!name)
        return (NULL);
    
    p = name + len - 1;
    while (strchr(name, 'X')!=NULL)
    {
        unsigned int random = ft_rand(num);
        unsigned int idx = num / random % (unsigned int)(sizeof(charset)-1);
        *p = charset[idx];
        num/=10;
        if (num==0)
            num = num_keep;
    }
    return (name);
}

int ft_mkstmpfd(char *template, char **filename)
{
    size_t try_limit = 5000;
    if (template==NULL)
        return (-1);
    while (1)
    {
        *filename = ft_mkstmp(template);
        int fd = open(*filename, O_CREAT|O_WRONLY|O_EXCL);
        if (fd>=0)
            return (fd);
        try_limit--;
        if (try_limit==0)
            return -1;
    }
    return (-1);
}
