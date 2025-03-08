NAME := pipex
BONUS_NAME := pipex_bonus
CC := cc
CFLAGS := -Wall -Wextra -Werror
INCLUDES := -I include -I libft
LDFLAGS := -L libft -lft
LIBFT := libft/libft.a

SRC_MAIN := $(filter-out src/main_bonus.c, $(wildcard src/*.c))
SRC_BONUS := $(filter-out src/main.c, $(wildcard src/*.c))
OBJ_DIR := obj
OBJ_MAIN := $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_BONUS := $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC_BONUS))

all: $(NAME)

$(NAME): $(OBJ_MAIN) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_MAIN) $(LDFLAGS) -o $(NAME)

bonus: $(BONUS_NAME)

$(BONUS_NAME): $(OBJ_BONUS) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_BONUS) $(LDFLAGS) -o $(BONUS_NAME)

$(LIBFT):
	make -C libft complete

$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	make -C libft fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all bonus

.PHONY: all bonus clean fclean re
