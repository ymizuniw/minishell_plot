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
    TILDE,
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


typedef enum e_reserved
{
    EXCLAMATION,
    DO,
    ESAC,
    IN,
    OPEN_BRACE,
    CLOSE_BRACE,
    DONE,
    FI,
    THEN,
    ELIF,
    FOR,
    UNTIL,
    CASE,
    ELSE,
    IF,
    WHILE
} t_reserved;
//RULES
//1.the first word of a command
//2.the first word following one of the reserved words other than case, for, or in
//3.the third word in a CASE command (only IN is valid in the case)
//4.the third word in a for command (only IN and DO are valid in this case)
//In addition, "[[  ]]", function, select are resered words in some implementations.


//2.5 Parameters and Variables
//A parameter is set if it has an assigned value(null is valid value). Once a variable is set, it can only be unset by using the unset special built-in command.

//2.5.1 Positional Parameters
//A positional parameter is a parameter denoted by the decimal value represented by one or more digits, other than the single digit 0.
//The digits denoting the positional parameters shall always be interpreted as a decimal value, even if there is a loading zero. When a positional parameter with more than one digit is specified,
//the application shall enclose the digits in braces. Positional parameters are initially assigned when the shell is invoked,
//temporarily replaced when a shell function is invoked, and can be reassigned with the set special built-in command.

//2.5.2 Special Parameters
//'*' : Expands to the positional parameters, starting from one, initially producing one field for each positional parameter that is set. When the expansion occurs in a context where field splitting will be performed, any empty fields may be discarded and each of the non-empty fields shall be further split as described in Field Splitting.
//When the expansion occurs in a contexxt where field splitting will not be performed, the initial fields shall be joined to form a single field with the value of each parameter separated by the first character of the IFS variables if IFS contains at least onee character, or separated by a <space> if IFS is unset, or with no separation if IFS is set to a null string.
//'?' : Expands to the decimal exit status of the most recent pipeline.(See Pipelines).
//'$' : Expands to the decimal process ID of the invoked shell. In a subshell (see Shell Execution Environment), '$' shall expand to the same value as that of the current shell.
//'0' : zero. Expands to the name of the shell or shell script. See sh for a detailed description of how this name is derived.

//2.5.3 Shell Variables
//Variables shall be initialized from the environment and can be given new values with variable assignment commands. If a variable is initialized from the environment, it shall be marked for export immediately; see "export" special built-in. New variables can be defined and intialized with variable assignements, with the read or getopts utilities, with the name parameter in a for loop,
//with the ${name= word} expansion, or with other mechanisms provided as implementation extensions.
//The following variables shall affect the execution of the shell:
/*
    ENV
    [UP] The processing of the ENV shell variable shall be supported if the system supports the User Portability Utilities option.
    This variable, when and only when an interactive shell is invoked, shall be subjected to parameter expansion (see Parameter Expansion) by the shell and the rexulting value shall be used as a pathname of a file containing shell commands to execute in the current environment.
    Thefile need not be executable. If the expanded value of ENV is not an absolute pathname, the results are unspecified. ENV shall be ignored if the user's real and effective user IDs or real and effective group IDs are different.

    HOME
    The pathname of the user's home directory. The contents of HOME are used in tilde expansion (see Tilde Expansion).

    IFS
    A string treated as a list of characters that is used for field splitting, expansion ofthe '*' special parameter, and to split lines into fields with the read utility.
    If the value of IFS includes any bytes that do not form part of a valid character, the results of field splitting, expansion of '*', and use of the "read" utility are unspecified.
    if IFS is not set, it shall behave as normal for an unset variable, except that field splitting by the shell and line splitting by the "read" utility shall be performed as if the value of IFS is <space> <tab> <newline>; see "Field Splitting".
    The shell shall set IFS to <space> <tab> <newline> when it is invoked.

    LANG
    Provide a default value for the internationalization variables that are unset or null. (See XBD Internationalization Variables for the precedence of the internationalization variables used to determine the values of locale categories.)

    LC_ALL
    The value of this variable overrides the LC_* variables and LANG, as described in XBD Environment Variables.
    LC_COLLATE
        Determine the behavior of range expressions, equivalence classes, and multi-charactercollating elements within pattern matching.
    LC_CTYPE
        Determine the interpretation of sequences of bytes of text data as characters(for example, single-byte as oppoesed to multi-byte characters), which characters are defined as letters (character class alpha) and <blank> characters (characterr class "blank"), and the behavior of character classes within pattern matching. Changing the value of LC_CTYPE after the shell has started shall not affect the lexical processing of shell commands in the current shell execution environment or its subshells. Invoking a shell script or performing "exec sh" subjects the new shell to the changes in LC_CTYPE.
    LC_MESSAGES
        Determine the language in which messages should be written.
    LINENO
        Set byt the shell to a decimal number representing the current sequential line number (numbered starting with 1) within a script or function before it executes each command. If the user unsets or resets LINEN O, the variable may lose its special meaning for the line of the shell.
        If the shell is not currently executing a script or functio, the value of LINENO is unspecified.
        This volume of POSIX.1-2017 specifies the effects of the variable only for systems supporting the User Portability Utilities option.
    NLSPATH
        Determine the location of message catalogs for the processing of LC_MESSAGES.
    NLSPATH
        Determine the location of message catalogs for the processing of LC_MESSAGES.
    PATH
        A string formatted as described in XBD Environment Variables, used to effect comamnd interpretation; see Command Search and Execution.
    PPID
        Set byt the shell to the decimal value of its parent process ID during initialization of the shell. In a subshell (see "Shell Execution Environment"), PPID shall be set to the same value as that of the aprent of the current shell. For example, echo $ PPID and (echo $ PPID) would produce the same value.
    PS1
        Each time an interactive shell is ready to read a command, the value of this variable shall be subjected to parameter expansion and written to standard error. The default value shall be "$ ".
        For users who have specific additional implementation-defined privileges, the default may be another, implementation-defined value. The shell shall replace each instance of the character '!' in PS1 with the history file number of the next command to be typed. Escaping the '!' with another '!' (that is , "!!") shall place the literal characterr '!' in the prompt.
        This volume of POSIX.1-2017 specifies the effects of the variable only for systems supporting the User Portability Utilities option.
    PS2
        Each time the user enters a <newline> prior to completing a command line in.an interactive shell, the value of this variable shall be subjected to parameter expansion and written to standard error. The default value is "> ". This volume of POSIX.1-2017 specifies the effects of the variable only for sywstems supporting the User Portability Utilities option.
    PS4
        When an execution trace (set -x) is being performed in an interactive shell, before each line in the execution trace, the value of this variable shall be subjected to parameter expansion and written to standard error. The default value is "+ ". This volume of POSIX.1-2017 specifies the effects of the variable only for systems supporting the User Portability Utilities option.
    PWD
        Set by the shell and by the "cd" utility. In the shell the value shall be initialized from the environment as follows. If a value for PWD is passed to the shell in the environment when it is executed, the value is an absolute pathname of the current working directory that is not longer than {PATH_MAX} bytes including the terminating null byte, and the value does not contain any components that are dot or dot-dot, then the shell shall set PWD to the value from the environment. Otherwise, if a value for PWD is passed to the shell in the environment when it is executed, the value is an absolute pathname of the current working directory, and the value does not contain any componets that are dot or dot-dot, then it is unspecified whehter the shell sets PWD to the value from the environment or sets PWD to the pathname that would be output by "pwd -P".
        In cases where PWD is set to the symbolic link. In cases where PWD is set to the pathname that would be output by "pwd -P", if there is insufficient permission on the current working directory, or on any parent of that directory, to determine what that pathname would be, the value of PWD is unspecified. Assignments to this variable may be ignored. If an application sets or unsets the value of PWD, the behaviors of the "cd" and "pwd" utilities are unspecified.
    
*/


//
//2.6 Word Expansion
/*
    This section descibes the various expansions that are perfomed on words. Not all expansions are performed on every word, as explained in the following sections.
    Tilde expansions, parameter expansions, command substitutions, arthmetic expansions, and quote removeals that occur within a single word expand to a single field.
    It is only field splitting or pathname expansion that can create multiple fieldsfrom a single word. The single exception to this rule is the expansion of the special parameter '@' within double-quotes, as described in "Special Parameters".

    The order of word expansion shall be follows:
    1.Tilde expansion, parameter expansion, command substitution, and arithmetic expansion, beggining to end.
    2.Field splitting shall be performed.
    3.Pathname expansion shall be performed, unless "set -f" is in effect.
    4.Quote removal shall always be performed last.
    
    The expansion shall be performed in the same shell environment as that in which the command is executed.
    If the completed expansion results in an empty field, that empty field shall be deleted from the list of the field,
    unless the original word contained single-quote or double-quote characters.
    
    The '$' is used to introduce parameter expansion, command substitution, or arithmetic evaluation.
    If an unquoted '$' is followed by a characterr that is not one of the following.
    ・A numeric character
    ・The name of a special parameters
    ・A valid first character of a variable name
    ・A left curly bracket '{'
    ・A left parenthesis '(' 
    the result is unspecified.
*/

// 2.6.1
/*
    Tilde Expansion
        A "tilde-prefix" consists of an unquoted <tilde> character at the beginning of a word, followed by all of the characters preceeding the first unquoted <slash> in the word,
        or all the characters in the word if there is no <shash>.
        In an assignment, multiple tilde-prefixes can be used -: at the beginning of the word, following any unquoted <colon>, or both.
        A tilde-prefix in an assignment is terminated by the first unquoted <colon> or <slash>.
        If non of the characters in the tilde-prefix are quoted, the characters in the tilde-prefix following the <tilde> are treated as a possible login name from the user database.
        A portable login name cannot contain characters outside the set given in the description of the LOGNAME environment vairable in XBD. If the login name is null (that HOME. if HOME is unset, the results are unspecified. Otherwise, the tilde-prefix shall be replaced by a pathname of the inital working directory associated with the login name obtained using the getpwnam()) function  as defined in the System interfaces volume of POSIX.1-2017. If the system does not recognize the login name, the results are undefined.

        The pathname resulting from tilde expansion shall be treated as if quoted to prevdent it being alterd by field splitting and pathname expansion.
*/

// 2.6.1 Parameter Expansion
/*
    The format for parameter expansion is as follows: 
    ${expressions}
    where expression consists of all charactrers until the matching '}'. Any '}' excaped by a <backslash> or within a quoted string, and characters in embedded arithmetic expansions, command substitutions, and variable expansions, shall not be examined in detremining the matching '}'.
    The simplestform for parameter expansion is:
    ${parameter}
    The value if any, of parameters shall be substituted.
*/