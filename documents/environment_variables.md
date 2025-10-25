$PPID

Builtin commands that are invoked as part of a
       pipeline are also executed in a subshell environment.  Changes made to the subshell environment cannot affect the
       shell's execution environment.

When bash invokes an external command, the variable _ is set to the full file name of the command and passed to
       that command in its environment.


//shell builtin
Shell builtin commands return a status of 0 (true) if successful, and non-zero (false) if an error occurs while
they execute.  All builtins return an exit status of 2 to indicate incorrect usage.

Bash itself returns the exit status of the last command executed, unless a syntax error occurs, in which case it
exits with a non-zero value.  See also the exit builtin command below.