NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
      ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
      ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
      ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
      ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
      ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
      ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c

EXTRA_SRC = ft_strarray_dup.c ft_strcmp.c ft_error_exit.c ft_free_error_exit.c
BONUS_SRC = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
            ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
            ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
GNL_SRC = get_next_line.c
PRINTF_SRC = ft_printf.c ft_putargs.c

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)
EXTRA_OBJ = $(EXTRA_SRC:.c=.o)
GNL_OBJ = $(GNL_SRC:.c=.o)
PRINTF_OBJ = $(PRINTF_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

bonus: $(NAME) $(BONUS_OBJ) .bonus

extra: $(NAME) $(EXTRA_OBJ) .extra

gnl: $(NAME) $(GNL_OBJ) .gnl

printf: $(NAME) $(PRINTF_OBJ) .printf

complete: all bonus extra gnl printf

.bonus: $(BONUS_OBJ)
	ar rcs $(NAME) $(BONUS_OBJ)
	touch .bonus

.extra: $(EXTRA_OBJ)
	ar rcs $(NAME) $(EXTRA_OBJ)
	touch .extra

.gnl: $(GNL_OBJ)
	ar rcs $(NAME) $(GNL_OBJ)
	touch .gnl

.printf: $(PRINTF_OBJ)
	ar rcs $(NAME) $(PRINTF_OBJ)
	touch .printf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ) $(EXTRA_OBJ) $(GNL_OBJ) $(PRINTF_OBJ)
	rm -f .bonus .extra .gnl .printf

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus extra gnl printf complete
