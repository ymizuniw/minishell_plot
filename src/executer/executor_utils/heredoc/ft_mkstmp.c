#include "../../../../includes/minishell.h"

int ft_rand(void)
{
    int a = 1003;
    int m = 2147483647;
    int seed = (a * seed) % m;
    int random = seed / m;
    return (random);
}

char *ft_mkstmp(char *template, int pid)
{
    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char *p;
    size_t len = strlen(template);
    int pid_keep = pid;

    char *name  = strdup(template);
    if (!name)
        return (NULL);
    
    p = name + len - 1;
    while (strchr(name, 'X')!=NULL)
    {
        int random = ft_rand();
        int idx = pid / random % (int)(sizeof(charset)-1);
        *p = charset[idx];
        pid/=10;
        if (pid==0)
            pid = pid_keep;
    }
    return (name);
}

// static int
// _gettemp(char *path, int *doopen, int domkdir, int slen)
// {
// 	char *start, *trv, *suffp;
// 	struct stat sbuf;
// 	int rval;
// 	pid_t pid;
// 	if (doopen && domkdir) {
// 		errno = EINVAL;
// 		return(0);
// 	}
// 	for (trv = path; *trv; ++trv)
// 		;
// 	trv -= slen;
// 	suffp = trv;
// 	--trv;
// 	if (trv < path) {
// 		errno = EINVAL;
// 		return (0);
// 	}
// 	pid = getpid();
// 	while (trv >= path && *trv == 'X' && pid != 0) {
// 		*trv-- = (pid % 10) + '0';
// 		pid /= 10;
// 	}
// 	while (trv >= path && *trv == 'X') {
// 		char c;
// 		pid = (arc4random() & 0xffff) % (26+26);
// 		if (pid < 26)
// 			c = pid + 'A';
// 		else
// 			c = (pid - 26) + 'a';
// 		*trv-- = c;
// 	}
// 	start = trv + 1;
// 	/*
// 	 * check the target directory; if you have six X's and it
// 	 * doesn't exist this runs for a *very* long time.
// 	 */
// 	if (doopen || domkdir) {
// 		for (;; --trv) {
// 			if (trv <= path)
// 				break;
// 			if (*trv == '/') {
// 				*trv = '\0';
// 				rval = stat(path, &sbuf);
// 				*trv = '/';
// 				if (rval != 0)
// 					return(0);
// 				if (!S_ISDIR(sbuf.st_mode)) {
// 					errno = ENOTDIR;
// 					return(0);
// 				}
// 				break;
// 			}
// 		}
// 	}
// 	for (;;) {
// 		if (doopen) {
// 			if ((*doopen =
// 			    open(path, O_CREAT|O_EXCL|O_RDWR, 0600)) >= 0)
// 				return(1);
// 			if (errno != EEXIST)
// 				return(0);
// 		} else if (domkdir) {
// 			if (mkdir(path, 0700) == 0)
// 				return(1);
// 			if (errno != EEXIST)
// 				return(0);
// 		} else if (lstat(path, &sbuf))
// 			return(errno == ENOENT ? 1 : 0);
// 		/* tricky little algorithm for backward compatibility */
// 		for (trv = start;;) {
// 			if (!*trv)
// 				return (0);
// 			if (*trv == 'Z') {
// 				if (trv == suffp)
// 					return (0);
// 				*trv++ = 'a';
// 			} else {
// 				if (isdigit(*trv))
// 					*trv = 'a';
// 				else if (*trv == 'z')	/* inc from z to A */
// 					*trv = 'A';
// 				else {
// 					if (trv == suffp)
// 						return (0);
// 					++*trv;
// 				}
// 				break;
// 			}
// 		}
// 	}
// 	/*NOTREACHED*/
// }
