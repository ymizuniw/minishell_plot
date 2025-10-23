
/*
  <heredoc_expand()>
  if (redirectee->word == 0 || redirectee->word[0] == '\0')
    {
      if (lenp)
        *lenp = 0;
      return (redirectee->word);
    }

  /* Quoted here documents are not expanded */
//   if (ri != r_reading_string && (redirectee->flags & W_QUOTED))
//     {
//       if (lenp)
//         *lenp = STRLEN (redirectee->word);
//       return (redirectee->word);
//     }


// /* Write HEREDOC (of length HDLEN) to FD, returning 0 on success and ERRNO on
//    error. Don't handle interrupts. */
// static int
// heredoc_write (int fd, const char *heredoc, size_t herelen)
// {
//   ssize_t nw;
//   int e;

//   errno = 0;
//   nw = write (fd, heredoc, herelen);
//   e = errno;
//   if (nw != herelen)
//     {
//       if (e == 0)
// 	e = ENOSPC;
//       return e;
//     }
//   return 0;
// }

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
// #if HEREDOC_PARANOID
//   struct stat st1, st2;
// #endif

//   /* Expand the here-document/here-string first and then decide what to do. */
//   document = heredoc_expand (redirectee, ri, &document_len);

//   /* If we have a zero-length document, don't mess with a temp file */
//   if (document_len == 0)
//     {
//       fd = open ("/dev/null", O_RDONLY);
//       r = errno;
//       if (document != redirectee->word)
// 	FREE (document);
//       errno = r;
//       return fd;
//     }

//   if (shell_compatibility_level <= 50)
//     goto use_tempfile;

// #if HEREDOC_PIPESIZE
//   /* Try to use a pipe internal to this process if the document is shorter
//      than the system's pipe capacity (computed at build time). We want to
//      write the entire document without write blocking. */
//   if (document_len <= HEREDOC_PIPESIZE)
//     {
//       if (pipe (herepipe) < 0)
// 	{
// 	  /* XXX - goto use_tempfile; ? */
// 	  r = errno;
// 	  if (document != redirectee->word)
// 	    free (document);
// 	  errno = r;
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
//       if (document != redirectee->word)
// 	free (document);
//       close (herepipe[1]);
//       if (r)			/* write error */
// 	{
// 	  close (herepipe[0]);
// 	  errno = r;
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
//       r = errno;
//       FREE (filename);
//       if (document != redirectee->word)
// 	FREE (document);
//       errno = r;
//       return (fd);
//     }

//   fchmod (fd, S_IRUSR | S_IWUSR);
//   SET_CLOSE_ON_EXEC (fd);

//   errno = r = 0;		/* XXX */
//   r = heredoc_write (fd, document, document_len);
//   if (document != redirectee->word)
//     FREE (document);

//   if (r)
//     {
//       close (fd);
//       unlink (filename);
//       free (filename);
//       errno = r;
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

// #if HEREDOC_PARANOID
//   /* We can use same_file here to check whether or not fd and fd2 refer to
//      the same file, but we don't do that unless HEREDOC_PARANOID is defined. */
//   if (fstat (fd, &st1) < 0 || S_ISREG (st1.st_mode) == 0 ||
//       fstat (fd2, &st2) < 0 || S_ISREG (st2.st_mode) == 0 ||
//       same_file (filename, filename, &st1, &st2) == 0)
//     {
//       unlink (filename);
//       free (filename);
//       close (fd);
//       close (fd2);
//       errno = EEXIST;
//       return -1;
//     }
// #endif

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