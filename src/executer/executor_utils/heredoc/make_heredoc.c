
#include "../../../../includes/minishell.h"

char *ext_unit(char *src, size_t start, size_t end)
{
  char *unit = malloc(sizeof(char)*(end-start+2));
  strncpy(unit, &src[start], end-start+2);
  return (unit);
}

int join_value(char **res, char *value, size_t size1, size_t size2)
{
  *res = realloc(*res, sizeof(char)*(size1 + size2 + 1));
  if (res==NULL)
    return (0);
  strncpy(*res, value, size2+1);
  return (1);
}

//heredoc mode will expand all environment variables and quotations. It means that it won't recognize quotations.
char *heredoc_value_expansion(char *line, bool in_quote, size_t line_len)
{
  size_t i=0;
  char *start = line;
  // char *end  = start;
  char *res = NULL;
  // size_t res_len = 0;
  size_t start_idx = 1;
  size_t end_idx = 1;
  (void)in_quote;
  while (i<line_len)
  {
    char *doller = strchr(start, '$');
    if (doller!=NULL)
    {
      while (&doller[end_idx]<&line[line_len])
      {
        char *unit = ext_unit(doller, start_idx, end_idx);
        if (unit==NULL)
          break ;
        // char *var = ft_getenv(unit);
        char *var = getenv(unit);
        if (var != NULL)
        {
          if (join_value(&res, unit, strlen(res), strlen(var)))
            return (NULL);
          i += end_idx;
          break ;
        }
        end_idx++;
      }
    }
    start = &start[end_idx]+1;
  }
  return (res);
}

char *heredoc_expansion(char *line, bool in_quote, size_t line_len)
{
  char *new = heredoc_value_expansion(line, in_quote, line_len);
  if (new==NULL)
      return (NULL);
  return (new);
}

int get_document(t_redir *hd, char **document, size_t *document_len)
{
  char *delim = hd->filename;
  size_t delim_len = strlen(delim);
  char *line = NULL;
  size_t line_len = 0;

  while (1)
  {
    line = readline("> ");
    if (strncmp(line, delim, delim_len)==0)
      return (1);
    if (line!=NULL)
      line_len = strlen(line);
    char *value = heredoc_expansion(line, hd->delim_quoted, line_len);
    size_t value_len = strlen(value);
    if (join_value(document, value, *document_len, value_len)<0)
      return (-1);
    *document_len += value_len;
    free(line);
    free(value);
  }
  return (-1);
}

ssize_t heredoc_write_to_fd(int herepipe[2], char *document, size_t document_len)
{
  ssize_t wb = 0;
  wb = write(herepipe[1], document, document_len);
  if (wb!=(ssize_t)document_len)
    return (0);
  return (wb);
}

int get_tmp_fd(char *src, size_t size, char **filename)
{
  int tmp_fd = ft_mkstmpfd(HERE_TEMPLATE, filename);
  if (tmp_fd<0)
    return(-1);
  ssize_t wb = write(tmp_fd, src, size);
  if (wb!=(ssize_t)size)
  {
    close(tmp_fd);
    return (-1);
  }
  return tmp_fd;
}

int make_heredoc(t_redir *hd)
{
  // char *delim = hd->filename;
  // size_t delim_len = strlen(delim);
  char *line = NULL;
  // size_t line_len = 0;
  char *document=NULL;
  size_t document_len = 0;
  char *filename=NULL;
  int fd;

  if (get_document(hd, &document, &document_len)<0)
    return (-1);
  if (document_len==0)
  {
    fd = open("dev/null", O_RDONLY);
    if (fd<0)
      perror("open: ");
    free(document);
    return (fd);
  }
  if (document_len + 1 <= HERE_PIPE_SIZE)
  {
    int herepipe[2];
    int ret = pipe(herepipe);
    if (ret<0)
    {
      perror("pipe: ");
      free(document);
    }
    ssize_t wb = heredoc_write_to_fd(herepipe, document, document_len);
    close(herepipe[1]);
    if (wb == 0)
    {
      close(herepipe[0]);
      unlink (filename);
      free (filename);
      return (-1);
    }
    return (herepipe[0]);
  }
  else
  {
    int tmp_fd = get_tmp_fd(document, document_len, &filename);//get tmp_fd and write buffer content to it.
    if (tmp_fd<0)
    {
      free(line);
      free(document);
      return (-1);
    }
    int fd = open(filename, O_RDONLY);
    if (fd<0)
    {
      perror("open: ");
      close(tmp_fd);
      unlink(filename);
      free(filename);
      return (-1);
    }
    close(tmp_fd);
    return (fd);
  }
  return (-1);
}

/* Create a temporary file or pipe holding the text of the here document
   pointed to by REDIRECTEE, and return a file descriptor open for reading
   to it. Return -1 on any error, and make sure errno is set appropriately. */

// static int
// here_document_to_fd (WORD_DESC *redirectee, enum r_instruction ri)
// {
//   char *filename;
//   int r, fd, fd2, herepipe[2];
//   char *document;
//   size_t document_len;

//   /* Expand the here-document/here-string first and then decide what to do. */
//   document = heredoc_expand (redirectee, ri, &document_len);

//   /* If we have a zero-length document, don't mess with a temp file */
//   if (document_len == 0)
//     {
//       fd = open ("/dev/null", O_RDONLY);
//       return fd;
//     }

// #if HEREDOC_PIPESIZE
//   /* Try to use a pipe internal to this process if the document is shorter
//      than the system's pipe capacity (computed at build time). We want to
//      write the entire document without write blocking. */
//   if (document_len <= HEREDOC_PIPESIZE)
//     {
//       if (pipe (herepipe) < 0)
// 	{
// 	  return (-1);
// 	}

// #if defined (F_GETPIPE_SZ)
//       if (fcntl (herepipe[1], F_GETPIPE_SZ, 0) < document_len)
// 	{
// 	  close (herepipe[0]);
// 	  close (herepipe[1]);
// 	  goto use_tempfile;
// 	}
// #endif

//       r = heredoc_write (herepipe[1], document, document_len);
//       close (herepipe[1]);
//       if (r)			/* write error */
// 	{
// 	  close (herepipe[0]);
// 	  return (-1);
// 	}
//       return (herepipe[0]);
//     }
// #endif

// use_tempfile:

//   /* TAG: use anonfiles here in a future version. */
//   fd = sh_mktmpfd ("sh-thd", MT_USERANDOM|MT_USETMPDIR, &filename);

//   /* If we failed for some reason other than the file existing, abort */
//   if (fd < 0)
//     {
//       FREE (filename);
//       return (fd);
//     }

//   r = heredoc_write (fd, document, document_len);

//   if (r)
//     {
//       close (fd);
//       unlink (filename);
//       free (filename);
//       return (-1);
//     }

//   /* In an attempt to avoid races, we close the first fd only after opening
//      the second. */
//   /* Make the document really temporary.  Also make it the input. */
//   fd2 = open (filename, O_RDONLY|O_BINARY, 0600);

//   if (fd2 < 0)
//     {
//       r = errno;
//       unlink (filename);
//       free (filename);
//       close (fd);
//       errno = r;
//       return -1;
//     }


//   close (fd);
//   if (unlink (filename) < 0)
//     {
//       r = errno;
//       close (fd2);
//       free (filename);
//       errno = r;
//       return (-1);
//     }

//   free (filename);

//   fchmod (fd2, S_IRUSR);
//   return (fd2);
// }
