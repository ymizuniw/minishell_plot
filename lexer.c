#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//special char set
typedef enum e_meta_char
{
    PIPE,
    AND,
    SEMI_COLON,
    LARGER,
    SMALLER,
    BRACE_OPEN,
    BRACE_CLOSE,
    DOLLER,
    BACK_QUOTE,
    SINGLE_QUOTE,
    SPACE,
    TAB,

    NEWLINE,
    ASTALISK,
    QUESTION,
    BRACE_LARGE,
    SHARP,
    TILDA,
    EQUAL,
    PERCENT
} t_meta_char;

//IF \ COMESE:
//1.remain itself : almost all
//2.vanish : newline. below is the ex in zsh.
// ymizuniw@Yugos-MacBook-Air minishell_plot % echo "\\n" | cat -e
// $
// $

//single quote
//single quotes keeps string whithin it literal.
//nest of single quote cannot be tolerated.
//

//double quote
//single quotes keeps string whithin it literal with except that $,back_quote, and backslash.
//as a whole, you should refer to the item of "parameter expansion".
//$ : won't affected by the double quote. keep its expansion value. "${"->'}' pattern matching, then keep the value as the same as former one.
