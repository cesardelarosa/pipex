NAME = pipex

SRC_DIR = src
BONUS_DIR = bonus
OBJ_DIR = obj
LIBFT_DIR = ./libft
INC_DIR = ./include

SRCS = pipex.c
SRCS_COMMON = execution.c
SRCS_BONUS = pipex_bonus.c heredoc_bonus.c utils_bonus.c parse_bonus.c struct_bonus.c

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS)) \
	   $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS_COMMON))
OBJS_BONUS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS_BONUS)) \
			 $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS_COMMON))

LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra -I$(LIBFT_DIR)

all: $(NAME)

bonus: $(OBJS_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(NAME) $(OBJS_BONUS) -L$(LIBFT_DIR) -lft

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make complete -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all bonus clean fclean re
