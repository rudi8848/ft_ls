NAME = ft_ls
OBJ = $(SRC:.c=.o)
SRC = ft_ls.c ft_printf.c
LIB = libft/libft.a

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
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
