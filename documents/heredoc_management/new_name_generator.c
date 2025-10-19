


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
            if (charset[cur])
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