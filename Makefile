# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ksevciko <ksevciko@student.42prague.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/13 12:27:43 by jasminelage       #+#    #+#              #
#    Updated: 2025/10/03 00:02:07 by ksevciko         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
# DFLAGS = -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
OBJ_DIR = obj

SOURCES =	clean_up.c \
			command_utils.c \
			commands.c \
			environment.c \
			errors.c \
			execute.c \
			expand_len.c \
			expand.c \
			heredoc_expand.c \
			heredoc.c \
			initialize_minishell.c \
			main.c \
			parse.c \
			pipes.c \
			quotes_handling.c \
			quotes_handling_utils.c \
			redirect.c \
			remove_quotes.c \
			signals.c \
			tmp_file.c \
			token_define.c \
			token_type.c \
			
OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)/%.o)

# ----------------------------- for MAC compiling -----------------------------
# the GNU library is not installed on Mac by defult
# this looks up what systhem I am on and finds the library path accordingly
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS Homebrew readline prefix (adjust if your brew is in /usr/local)
    READLINE_PREFIX := /opt/homebrew
    CPPFLAGS += -I$(READLINE_PREFIX)/include
    LDFLAGS += -L$(READLINE_PREFIX)/lib -lreadline -liconv
else ifeq ($(UNAME_S),Linux)
    # Linux usually has readline in default system paths
    CPPFLAGS +=
    LDFLAGS += -lreadline
endif
# -----------------------------------------------------------------------------

all: $(LIBFT) $(NAME)
# ----------------------------- for MAC compiling -----------------------------
# the GNU library is not installed on Mac by defult
# this looks up what systhem I am on and finds the library path accordingly
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    # macOS Homebrew readline prefix (adjust if your brew is in /usr/local)
    READLINE_PREFIX := /opt/homebrew
    CPPFLAGS += -I$(READLINE_PREFIX)/include
    LDFLAGS += -L$(READLINE_PREFIX)/lib -lreadline -liconv
else ifeq ($(UNAME_S),Linux)
    # Linux usually has readline in default system paths
    CPPFLAGS +=
    LDFLAGS += -lreadline
endif
# -----------------------------------------------------------------------------

all: $(LIBFT) $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(LDFLAGS) -o $(NAME)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -I $(LIBFT_DIR) -c $< -o $@
	$(CC) $(CFLAGS) $(CPPFLAGS) -I $(LIBFT_DIR) -c $< -o $@
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
