#include "../../includes/minishell.h"

void *xmalloc(size_t size)
{
    void *p = (void *)malloc(size);
    if (p==NULL)
        perror("malloc: ");
    return (p);
}

void *xcalloc(size_t size)
{
    void *p = xmalloc(size);
    memset(p, 0, size);
    return (0);
}
