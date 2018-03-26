/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 13:30:49 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 13:30:59 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

int			ft_set_option(char symb, t_opt *options)
{
	if (symb == 'a' || symb == 'l' || symb == 'R'
		|| symb == 'r' || symb == 't' || symb == '1' || symb == 'S')
	{
		if (symb == 'a')
			options->a = 1;
		if (symb == 'l')
			options->l = 1;
		if (symb == 'R')
			options->rr = 1;
		if (symb == 'r')
			options->r = 1;
		if (symb == 't')
			options->t = 1;
		if (symb == '1')
			options->c1 = 1;
		if (symb == 'S')
			options->sz = 1;
		return (1);
	}
	return (0);
}

t_opt		*ft_read_options(int argc, char **argv, t_opt *options)
{
	int				i;
	char			*ptr;

	i = 1;
	while (i < argc)
	{
		if (*argv[i] == '-')
		{
			ptr = argv[i];
			++ptr;
			while (*ptr)
			{
				if (!ft_set_option(*ptr, options))
				{
					ft_miniprintf("./ft_ls: illegal option '%c'\n", *ptr);
					ft_miniprintf("usage: ./ft_ls [-larRt1] [file ...]\n");
					exit(EXIT_FAILURE);
				}
				ptr++;
			}
		}
		i++;
	}
	return (options);
}

int			ft_ls(t_opt options, char *name)
{
	t_flist			*head;
	t_flist			*ptr;
	int				res;

	res = 0;
	head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!head)
	{
		perror("cannot allocate memory");
		exit(EXIT_FAILURE);
	}
	ptr = head;
	res = ft_read_args(name, options, &head);
	if (res)
	{
		head = ft_sort_flist(options, head);
		ft_print_flist(options, head);
		ft_delete_flist(options, &head);
		free(ptr);
		return (1);
	}
	return (0);
}

int			ft_parse_args(int argc, char **argv, t_opt *options)
{
	int				i;
	int				f;
	int				res;
	char			***ptr;

	res = 0;
	f = 0;
	i = 0;
	ptr = &argv;
	options = ft_read_options(argc, *ptr, options);
	while (++i < argc)
	{
		if (argv[i] && argv[i][0] != '-')
		{
			f++;
			res = ft_ls(*options, argv[i]);
		}
	}
	if (!f)
		res = ft_ls(*options, ".");
	return (res);
}

int			main(int argc, char **argv)
{
	t_opt			*options;
	int				res;

	options = NULL;
	options = (t_opt*)ft_memalloc(sizeof(t_opt));
	if (!options)
		perror("Error");
	if (argc > 1)
		res = ft_parse_args(argc, argv, options);
	else
		res = ft_parse_args(1, NULL, options);
	if (!res)
		return (-1);
	free(options);
	return (0);
}
