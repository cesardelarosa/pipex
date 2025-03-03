NAME        = pipex

INCLUDE_DIR = include
SRC_DIR     = src
BONUS_DIR   = bonus
OBJ_DIR     = obj
LIBFT_DIR   = libft
MODE_FILE   = .mode

MANDATORY_SRC   = pipex.c execution.c parse_args.c
MANDATORY_OBJ   = $(MANDATORY_SRC:%.c=$(OBJ_DIR)/%.o)

BONUS_SRC   = pipex_bonus.c parse_bonus.c pipeline_bonus.c execution_bonus.c utils_bonus.c
BONUS_OBJ   = $(BONUS_SRC:%.c=$(OBJ_DIR)/%.o)

CC        = cc
CFLAGS    = -Wall -Werror -Wextra
LDFLAGS   = -L $(LIBFT_DIR) -lft
INCLUDE   = -I $(INCLUDE_DIR) -I $(LIBFT_DIR)

GREEN     = \033[0;32m
BLUE      = \033[0;34m
YELLOW    = \033[1;33m
RED       = \033[0;31m
NC        = \033[0m

all: mandatory

mandatory:
	@if [ ! -f $(NAME) ] || [ ! -f $(MODE_FILE) ] || [ "$$(cat $(MODE_FILE))" != "mandatory" ] || [ "$$(find $(SRC_DIR) -name '*.c' -newer $(NAME))" != "" ]; then \
		printf "$(BLUE)[pipex] Linking and generating the MANDATORY executable...$(NC)\n"; \
		$(MAKE) re_mandatory; \
	else \
		printf "$(BLUE)[pipex] The mandatory version is already compiled.$(NC)\n"; \
	fi

bonus:
	@if [ ! -f $(NAME) ] || [ ! -f $(MODE_FILE) ] || [ "$$(cat $(MODE_FILE))" != "bonus" ] || [ "$$(find $(BONUS_DIR) -name '*.c' -newer $(NAME))" != "" ]; then \
		printf "$(BLUE)[pipex] Linking and generating the BONUS executable...$(NC)\n"; \
		$(MAKE) re_bonus; \
	else \
		printf "$(BLUE)[pipex] The bonus version is already compiled.$(NC)\n"; \
	fi

re_mandatory: $(OBJ_DIR) $(MANDATORY_OBJ) $(LIBFT_DIR)/libft.a
	@printf "$(BLUE)[pipex] Linking and generating the MANDATORY executable...$(NC)\n"
	$(CC) $(CFLAGS) $(MANDATORY_OBJ) -o $(NAME) $(LDFLAGS)
	@printf "mandatory\n" > $(MODE_FILE)
	@printf "$(GREEN)[pipex] MANDATORY compilation completed$(NC)\n"

re_bonus: $(OBJ_DIR) $(BONUS_OBJ) $(LIBFT_DIR)/libft.a
	@printf "$(BLUE)[pipex] Linking and generating the BONUS executable...$(NC)\n"
	$(CC) $(CFLAGS) $(BONUS_OBJ) -o $(NAME) $(LDFLAGS)
	@printf "bonus\n" > $(MODE_FILE)
	@printf "$(GREEN)[pipex] BONUS compilation completed$(NC)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@printf "$(YELLOW)[pipex] Compiling $<...$(NC)\n"
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR)
	@printf "$(YELLOW)[pipex] Compiling bonus $<...$(NC)\n"
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@printf "$(BLUE)[pipex] Creating objects directory...$(NC)\n"
	mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR) complete

clean:
	@printf "$(RED)[pipex] Cleaning objects...$(NC)\n"
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@printf "$(RED)[pipex] Removing executable and mode file...$(NC)\n"
	rm -f $(NAME)
	rm -f $(MODE_FILE)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all mandatory bonus re_mandatory re_bonus clean fclean re
