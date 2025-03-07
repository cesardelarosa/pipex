NAME := pipex
CC := cc

# Flags de compilación e inclusión
CFLAGS := -Wall -Wextra -Werror
INCLUDES := -I include -I libft

# Flags de linking (-L para bibliotecas)
LDFLAGS := -L libft -lft

# Librería de libft
LIBFT := libft/libft.a

# Archivos fuente y objeto
SRC := $(wildcard src/*.c)
OBJ_DIR := obj
OBJ := $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) $(LDFLAGS) -o $(NAME)

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
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
