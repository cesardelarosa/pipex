NAME = pipex
#NAME_BONUS = pipex_bonus

SRC_DIR = .
#BONUS_DIR = bonus
OBJ_DIR = obj
LIBFT_DIR = ./libft
#INCLUDE_DIR = ./include

SRCS = pipex.c #utils.c
SRCS_BONUS = $(wildcard $(BONUS_DIR)/*.c)

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS_BONUS = $(patsubst $(BONUS_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_BONUS))

LIBFT = $(LIBFT_DIR)/libft.a

CC = gcc
CFLAGS = -Wall -Werror -Wextra -I$(LIBFT_DIR) #-I$(INCLUDE_DIR)

all: $(NAME)

#bonus: $(NAME_BONUS)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft

$(NAME_BONUS): $(OBJS_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJS_BONUS) -L$(LIBFT_DIR) -lft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make complete -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all bonus clean fclean re
