// DEFINITIONS
//        The following definitions are used throughout the rest of this document.
//        blank  A space or tab.
//        word   A sequence of characters considered as a single unit by the shell.  Also known as a token.
//        name   A word consisting only of alphanumeric characters and underscores, and beginning with an alphabetic character or an underscore.  Also
//               referred to as an identifier.
//        metacharacter
//               A character that, when unquoted, separates words.  One of the following:
//               |  & ; ( ) < > space tab
//        control operator
//               A token that performs a control function.  It is one of the following symbols:
//               || & && ; ;; ( ) | <newline>
//metacharacter splits input into tokens, if not quoted.



word is that quotation-processed unit of string split by metachar.

<!-- ymizuniw@companion:~/minishell_plot$ << EOF cat | cat
> $USER
> EOF
ymizuniw -->

<!-- ymizuniw@companion:~/minishell_plot$ << 'EOF' cat | cat
> $USER
> EOF
$USER
 -->

