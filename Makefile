NAME=minishell
CC=cc 
CFLAGS= -Wall -Wextra -Werror
#-fsanitize=address
#-g
INC=includes
SRCS=$(DATA_MANAGEMENT) $(LEXER) $(PARSER) $(SIGNAL_MANAGEMENT)
OBJS=$(SRCS:%.c=%.o)

#DIRECTORIES
SRC_DIR=src
DATA_MANAGEMENT_DIR=$(SRC_DIR)/data_management
LEXER_DIR=$(SRC_DIR)/lexer
LEXER_UTILS_DIR=$(LEXER_DIR)/lexer_utils
PARSER_DIR=$(SRC_DIR)/parser
PARSER_UTILS_DIR=$(PARSER_DIR)/parser_utils
SIGNAL_MANAGEMENT_DIR=$(SRC)/signal_management

#SRC PATHS
DATA_MANAGEMENT=$(DATA_MANAGEMENT_DIR)/alloc_t.c $(DATA_MANAGEMENT_DIR)/free_t.c
LEXER=$(LEXER_DIR)/lexer.c $(LEXER_UTILS_DIR)/*.c
PARSER=$(PARSER_DIR)/parser.c $(PARSER_UTILS_DIR)/*.c
SIGNAL_MANAGEMENT=$(SIGNAL_MANAGEMENT_DIR)/signal_management.c


$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:;	rm -f $(OBJS)

fclean:; clean $(NAME)

re:; fclean all