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
    BRACKET,
    SHARP,
    TILDA,
    EQUAL,
    PERCENT
} t_meta_char;

typedef enum e_reserved
{
    EXCLAMATION,
    DO,
    ESAC,
    IN,
    OPEN_BRACE,
}

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

//back quote?

//backslash
//'\' followed by '$', <backquote>,'"', '\', <newline> retins its special meaning.
//>>additional: '@' has special meaning. refer to "Special Parameter".


//TOKEN RECOGNITION
//1.<if the EOF recognized> the current token shall be delimited.
//2.<if the following char is not quoted>if a combination of charactor that consists an operator, the following charactor shall be used as a part of the operator.
//3.<if the current char is '\', ''', '"'> && <if they are not quoted.> affects till the end of the quoted text.
//  3.5<in the token recognition process> no substitution shall be performed. no change shall be aplied in this process, except for <newline> joining. 
//  the token shall not be delimited by the end of the quoted field.
//4.<if the current char is '$', or 'backslash'> the shell shall indentify the candidates:
//'$', "${", "$(", "$((", and 'bachslash'.
//  4.5 <if nested quotes found> the shell shall recursively process it, without affecting the result of the result token.
/* in the token recognition process, it just classify the input materials and keep the token type information. */
// 4.5 the token shall no be delimited by the end of the substitution.
//5.<if the current char is no quoted and can be used as the first char of a new operator> the current token shall be delimited.
//6.<if the current char is <blank>> any token containing the previous charactor is delimited and the current char is discarded.
//7.<if the current char is a part of a word> the current char shall be appended to the word
//8.<if the current char is '#'> it and all subsequent chars except <newline>, shall be discarded as a comment. <newline> shall be valid.
//9.the current char is used as the start of a new word.


//ALIAS
//after recognizing the tokens, before applying the rules of the shell gramer, check whether the aliases are valid.
//below is the RESERVED WORDS. 
