NAME	=	ft_ls
OBJ		=	$(SRC:.c=.o)
SRC		=	ft_ls_help_functions.c ft_ls_sort.c ft_ls_main.c \
			ft_ls_print_list.c ft_ls_list_manage.c ft_ls_read.c \
			ft_ls_get_info.c ft_miniprintf.c ft_miniprintf_parsing.c\
			ft_miniprintf_conv.c ft_miniprintf_char.c

LIB		=	libft/libft.a
HEADERS	=	ft_ls.h ft_miniprintf.h

all: $(NAME)

$(NAME): $(OBJ) $(LIB) $(HEADERS)
	gcc -o $(NAME) -g3 -Wall -Wextra -Werror $(OBJ) -L. $(LIB)

$(OBJ): $(SRC) $(LIB)
	gcc -c -g3 $(SRC)

$(LIB):
	make -C libft

clean:
	/bin/rm -f $(OBJ)
	make clean -C libft

fclean: clean
	@/bin/rm -f $(NAME)
	make fclean -C libft

re: fclean all
