NAME = ft_ls
OBJ = $(SRC:.c=.o)
SRC = ft_ls.c
LIB = libftprintf/libftprintf.a

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
	gcc -o $(NAME) -Wall -Wextra -Werror $(OBJ) -L. $(LIB)

$(OBJ): $(SRC) $(LIB)
	gcc -c $(SRC)

$(LIB):
	make -C libftprintf

clean:
	/bin/rm -f $(OBJ)
	make clean -C libftprintf

fclean: clean
	@/bin/rm -f $(NAME)
	make fclean -C libftprintf

re: fclean all
