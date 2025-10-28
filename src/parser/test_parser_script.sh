#!/bin/bash

# cc  -Wall -Wextra -Werror parser_test.c parser.c parser_utils/*.c ../lexer/lexer.c ../lexer/lexer_utils/*.c ../data_management/*.c -o parser_unit_test
# ./parser_unit_test

cc  -g -Wall -Wextra -Werror parser_test.c parser.c parser_utils/*.c ../lexer/lexer.c ../lexer/lexer_utils/*.c ../data_management/*.c -o parser_unit_test
gdb ./parser_unit_test

# rm -f lexer_unit_test

# valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=parser.log ./test_parser_script.sh
