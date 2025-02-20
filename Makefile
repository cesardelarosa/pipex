NAME        = pipex

INCLUDE_DIR = include
SRC_DIR     = src
BONUS_DIR   = bonus
OBJ_DIR     = obj
LIBFT_DIR   = libft
MODE_FILE   = .mode

MANDATORY_SRC   = pipex.c execution.c
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
	@if [ -f $(MODE_FILE) ] && [ "$$(cat $(MODE_FILE))" = "mandatory" ]; then \
		echo "$(BLUE)[pipex] The mandatory version is already compiled.$(NC)"; \
	else \
		$(MAKE) re_mandatory; \
	fi

bonus:
	@if [ -f $(MODE_FILE) ] && [ "$$(cat $(MODE_FILE))" = "bonus" ]; then \
		echo "$(BLUE)[pipex] The bonus version is already compiled.$(NC)"; \
	else \
		$(MAKE) re_bonus; \
	fi

re_mandatory: $(OBJ_DIR) $(MANDATORY_OBJ) $(LIBFT_DIR)/libft.a
	@echo "$(BLUE)[pipex] Linking and generating the MANDATORY executable...$(NC)"
	$(CC) $(CFLAGS) $(MANDATORY_OBJ) -o $(NAME) $(LDFLAGS)
	@echo "mandatory" > $(MODE_FILE)
	@echo "$(GREEN)[pipex] MANDATORY compilation completed$(NC)"

re_bonus: $(OBJ_DIR) $(BONUS_OBJ) $(LIBFT_DIR)/libft.a
	@echo "$(BLUE)[pipex] Linking and generating the BONUS executable...$(NC)"
	$(CC) $(CFLAGS) $(BONUS_OBJ) -o $(NAME) $(LDFLAGS)
	@echo "bonus" > $(MODE_FILE)
	@echo "$(GREEN)[pipex] BONUS compilation completed$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "$(YELLOW)[pipex] Compiling $<...$(NC)"
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(BONUS_DIR)/%.c | $(OBJ_DIR)
	@echo "$(YELLOW)[pipex] Compiling bonus $<...$(NC)"
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@echo "$(BLUE)[pipex] Creating objects directory...$(NC)"
	mkdir -p $(OBJ_DIR)

$(LIBFT_DIR)/libft.a:
	$(MAKE) -C $(LIBFT_DIR) complete

clean:
	@echo "$(RED)[pipex] Cleaning objects...$(NC)"
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)[pipex] Removing executable and mode file...$(NC)"
	rm -f $(NAME)
	rm -f $(MODE_FILE)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all mandatory bonus re_mandatory re_bonus clean fclean re
