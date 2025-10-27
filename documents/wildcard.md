
wildcars for filename.
int fd = open(shell->pwd, O_RDONLY);
if (fd<0)
{
  perror("open");
  return (NULL);
}

struct dirent {
        __uint32_t d_fileno;            /* エントリのファイル番号 */
        __uint16_t d_reclen;            /* このレコードの長さ */
        __uint8_t  d_type;              /* ファイルタイプ、以下参照 */
        __uint8_t  d_namlen;            /* d_name の文字列長 */
#ifdef _POSIX_SOURCE
        char    d_name[255 + 1];        /* 名前はこの長さを越えてはならない */
#else
#define MAXNAMLEN       255
        char    d_name[MAXNAMLEN + 1];  /* 名前はこの長さを越えてはならない */
#endif
};

struct dirent {
    ino_t          d_ino;       /* inode 番号 */
    off_t          d_off;       /* オフセットではない; 注意を参照 */
    unsigned short d_reclen;    /* このレコードの長さ */
    unsigned char  d_type;      /* ファイル種別。全ファイルシステム */
                                   でサポートされているわけではない */
    char           d_name[256]; /* ファイル名 */
};

#include <dirent.h>

DIR *dir = opendir(path);
if (dir==NULL)
{
  perror("opendir);
  return NULL;
}

typedef struct s_wildcard{
  char name[256];
  char type == 0;//'d' or 'f'
  struct s_widlcard *next;
} t_wildcard;

struct dirent *dirp;

           dirp = opendir(path);
           if (dirp == NULL)
                return (ERROR);
           while ((dp = readdir(dirp)) != NULL) {
              if (content_type == file)
                add_file_name(dp->name);
              else
                add_dir_name(dp->name);
           }
           (void)closedir(dirp);
           return (NOT_FOUND);

wildcards for dirname.

just join '*' and arbitrary str.

ls *, ls */ : read dir
  dirname1:
    file1 file2 ...
  dirname2:
    file1 file2 ...

cat * : read file, if argv is dir, perror() is valid.


if the argment char *str; includes '*' without quotation,

pattern match will be:

    if (str[])

if (str[start]!='*')
{
    while (str[end]!='*')
        end++;
    end--;
    char *part = extract_word(str, start, end);
    find_filename_start_with(part, content_name_list);
    >>int mark_filename_with_part(char *part, t_wildcard *wd)
    {
        //use wd included idx for the start point to search.
        t_wildcard *cur = wd;
        while (cur!=NULL && memmem(cur->name, part)!=NULL)
        {
            mark_to_expand();turn the flag and record idx of the last char before '*'.
            cur=cur->next;
        }
    }
    i+=end;
}
while(str[i]=='*')
    i++;
start = i;
end = start;
while (str[end]!='*')
    end++;
mark_filename_with_part(char *part, t_wildcard *wd)

str = "str1*str2*"
>for each filename in s_wildcard object,  memmem() with str1, str2, shifting the start point.
