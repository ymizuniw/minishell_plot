



//plot1

#define unique_set "fdas4312ghkj5678"

char *gen_unique_name()
{
    char *name=NULL;
    int  rand=0;
    name=getenv("$PPID");
    rand=atoi(name);

    int mod=0;

    size_t i=0;
    char *tmp = malloc(sizeof(char)*(PATH_MAX+1));
    while (1)
    {
        //cpy mod idx from unique set to tmp,

        //check if it already exists
            rand -= 3;
            continue ;
        //if not exit
            return(tmp);
    }
}

//plot2
char *unique_filename(char *filename, size_t len, int *charset[], size_t times)
{
    char c;
    char n;
    char sign;

    size_t mod=0;
    size_t i=0;

    while (i<len)
    {
        mod = len%i;
        if (mod==0)
        {
            int cur = (int)filename[i];
            if (charset[cur]<times)
                filename[i]=
        }
        if (mod==1)
        {

        }
        if (mod==2)
        {

        }
    }

}


char *gen_filename(char *filename)
{
    while (1)
    {
        int fd = open(filename, O_CREAT | O_EXCL);
        if (fd < 0)
        {

        }
    }
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
