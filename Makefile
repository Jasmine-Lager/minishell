NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
# DFLAGS = -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJ_DIR = obj

SOURCES =	main.c initialize_var.c parsing.c \
			pipes.c commands.c redirecting.c \
			exec_with_pipes.c clean_up.c
			
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

all: $(LIBFT) $(NAME) clean

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) -lreadline -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(LIBFT_DIR) -c $< -o $@
# 	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
