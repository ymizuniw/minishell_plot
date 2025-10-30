Redirecting Input
       Redirection of input causes the file whose name results from the expansion of word to be opened for reading on file
       descriptor n, or the standard input (file descriptor 0) if n is not specified.

       The general format for redirecting input is:

              [n]<word

   Redirecting Output
       Redirection of output causes the file whose name results from the expansion of word to be opened for writing on
       file descriptor n, or the standard output (file descriptor 1) if n is not specified.  If the file does not exist it
       is created; if it does exist it is truncated to zero size.

       The general format for redirecting output is:

              [n]>word

       If the redirection operator is >, and the noclobber option to the set builtin has been enabled, the redirection
       will fail if the file whose name results from the expansion of word exists and is a regular file.  If the
       redirection operator is >|, or the redirection operator is > and the noclobber option to the set builtin command is
       not enabled, the redirection is attempted even if the file named by word exists.

Appending Redirected Output
       Redirection of output in this fashion causes the file whose name results from the expansion of word to be opened
       for appending on file descriptor n, or the standard output (file descriptor 1) if n is not specified.  If the file
       does not exist it is created.

       The general format for appending output is:

              [n]>>word
        
   Here Documents
       This type of redirection instructs the shell to read input from the current source until a line containing only
       word (with no trailing blanks) is seen.  All of the lines read up to that point are then used as the standard input
       for a command.

       The format of here-documents is:

              <<[-]word
                      here-document
              delimiter

       No parameter expansion, command substitution, arithmetic expansion, or pathname expansion is performed on word.  If
       any characters in word are quoted, the delimiter is the result of quote removal on word, and the lines in the here-
       document are not expanded.  If word is unquoted, all lines of the here-document are subjected to parameter
       expansion, command substitution, and arithmetic expansion.  In the latter case, the character sequence \<newline>
       is ignored, and \ must be used to quote the characters \, $, and `.

       If the redirection operator is <<-, then all leading tab characters are stripped from input lines and the line
       containing delimiter.  This allows here-documents within shell scripts to be indented in a natural fashion.
