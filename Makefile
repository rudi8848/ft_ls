# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/26 14:27:19 by gvynogra          #+#    #+#              #
#    Updated: 2018/03/26 14:27:28 by gvynogra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ft_ls
OBJ		=	ft_ls_help_functions.o ft_ls_sort.o ft_ls_main.o \
			ft_ls_print_list.o ft_ls_list_manage.o ft_ls_read.o \
			ft_ls_get_info.o ft_miniprintf_num.o ft_miniprintf_parsing.o\
			ft_miniprintf_conv.o ft_miniprintf_char.o ft_ls_bonus_sort.o\
			ft_miniprintf_num_align.o

SRC		=	src/ft_ls_help_functions.c src/ft_ls_sort.c src/ft_ls_main.c \
			src/ft_ls_print_list.c src/ft_ls_list_manage.c src/ft_ls_read.c \
			src/ft_ls_get_info.c src/ft_miniprintf_num.c src/ft_miniprintf_parsing.c\
			src/ft_miniprintf_conv.c src/ft_miniprintf_char.c src/ft_ls_bonus_sort.c\
			src/ft_miniprintf_num_align.c

LIB		=	libft/libft.a
INCL	=	includes/ft_ls.h includes/ft_miniprintf.h
FLAGS	=	-Wall -Wextra -Werror
all: $(NAME)

$(NAME): $(OBJ) $(LIB) $(INCL)
	gcc -o $(NAME) -g3 -Wall $(FLAGS) $(OBJ) -L. $(LIB)

$(OBJ): $(SRC) $(LIB)
	gcc -c -g3 $(FLAGS) $(SRC)

$(LIB):
	make -C libft

clean:
	/bin/rm -f $(OBJ)
	make clean -C libft

fclean: clean
	@/bin/rm -f $(NAME)
	make fclean -C libft

re: fclean all
