NAME := pipex
BONUS_NAME := pipex_bonus

CC := cc
CFLAGS := -Wall -Wextra -Werror

INCLUDES := -I include -I libft
LDFLAGS := -L libft -lft

LIBFT := libft/libft.a

SRC_DIR := src
OBJ_DIR := obj

COMMON_SRC_FILES = \
	command.c \
	execute.c \
	handle_redir_out.c \
	pipeline.c \
	pipeline_utils.c \
	error.c \
	handle_redir_in.c \
	parse_args.c \
	pipeline_execute.c

SRC_MAIN_FILES = \
	handle_redirs.c \
	main.c

SRC_BONUS_FILES = \
	handle_redirs_bonus.c \
	handle_redir_append_bonus.c \
	handle_heredoc_bonus.c \
	main_bonus.c

COMMON_SRC = $(addprefix $(SRC_DIR)/, $(COMMON_SRC_FILES))
SRC_MAIN = $(COMMON_SRC) $(addprefix $(SRC_DIR)/, $(SRC_MAIN_FILES))
SRC_BONUS = $(COMMON_SRC) $(addprefix $(SRC_DIR)/, $(SRC_BONUS_FILES))

OBJ_MAIN = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_BONUS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_BONUS))

YELLOW := \033[0;33m
GREEN  := \033[0;32m
RED    := \033[0;31m
NC     := \033[0m

all: $(NAME)
	@printf "$(GREEN)Done: $(NAME) built successfully$(NC)\n"

$(NAME): $(OBJ_MAIN) $(LIBFT)
	@printf "$(YELLOW)Linking $(NAME)...$(NC)\n"
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_MAIN) $(LDFLAGS) -o $(NAME)

bonus: $(BONUS_NAME)
	@printf "$(GREEN)Done: $(BONUS_NAME) built successfully$(NC)\n"

$(BONUS_NAME): $(OBJ_BONUS) $(LIBFT)
	@printf "$(YELLOW)Linking $(BONUS_NAME)...$(NC)\n"
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ_BONUS) $(LDFLAGS) -o $(BONUS_NAME)

$(LIBFT):
	@printf "$(YELLOW)Compiling libft...$(NC)\n"
	make -C libft complete

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@printf "$(YELLOW)Compiling %s...$(NC)\n" $<
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@printf "$(YELLOW)Creating directory $(OBJ_DIR)...$(NC)\n"
	mkdir -p $(OBJ_DIR)

clean:
	@printf "$(RED)Cleaning project...$(NC)\n"
	make -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@printf "$(RED)Full cleaning...$(NC)\n"
	rm -f $(NAME) $(BONUS_NAME) $(LIBFT)

re: fclean all bonus
	@printf "$(GREEN)Rebuilding complete!$(NC)\n"

.PHONY: all bonus clean fclean re
