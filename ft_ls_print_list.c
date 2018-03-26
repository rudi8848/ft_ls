/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_print_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 13:31:30 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 13:33:11 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			ft_get_nbr_columns(t_flist *head, int *cols, int *col_width)
{
	int				fd;
	struct winsize	argp;
	int				ret;
	int				width;

	fd = open("/dev/tty", O_RDONLY);
	if (fd < 0)
	{
		perror("Error open");
		exit(EXIT_FAILURE);
	}
	ret = ioctl(fd, TIOCGWINSZ, &argp);
	if (ret != 0)
	{
		perror("Error ioctl");
		exit(EXIT_FAILURE);
	}
	width = argp.ws_col;
	*col_width = ft_maxlen(head) + 1;
	if (width < *col_width)
		width = *col_width;
	*cols = width / *col_width;
}

void			ft_print_columns(t_flist *head, int total, int c, int r)
{
	int			cols;
	int			col_width;
	int			rows;
	t_flist		*ptr;
	int			j;

	ft_get_nbr_columns(head, &cols, &col_width);
	rows = ft_get_rows(total, cols);
	while (c < total && r < rows)
	{
		j = 0;
		while (j < cols)
		{
			if (c + rows * j < total)
			{
				ptr = ft_get_nth(head, c + rows * j);
				ft_miniprintf("%s%*s%s", ptr->color, -col_width, ptr->name, RESET);
			}
			j++;
		}
		write(1, "\n", 1);
		r++;
		c++;
	}
}

void			ft_print_elem(t_opt options, t_flist *head)
{
	int			total;

	total = ft_count_blocks(head);
	if (options.l)
	{
		ft_miniprintf("total %d\n", total);
		while (head)
		{
			ft_miniprintf("%-11s%4i%10s%5s%7d %-s%s %s%s%s\n",
			head->mode, head->nlink, head->user, head->group,
			head->size, head->date, head->color,
			head->name, RESET, head->ref);
			head = head->next;
		}
	}
	else if (options.c1)
	{
		while (head)
		{
			ft_miniprintf("%s%s%s\n", head->color, head->name, RESET);
			head = head->next;
		}
	}
	else
		ft_print_columns(head, ft_flist_counter(head), 0, 0);
}

void			ft_print_flist(t_opt options, t_flist *head)
{
	t_flist			*phead;
	struct stat		buf;

	phead = head;
	ft_print_elem(options, head);
	if (options.rr)
	{
		while (phead)
		{
			lstat(phead->path, &buf);
			if (buf.st_mode & S_IFDIR && buf.st_nlink > 1 &&
				!ft_strequ(phead->name, ".") &&
				!ft_strequ(phead->name, ".."))
				print_recursion(phead->path, options);
			phead = phead->next;
		}
	}
}

void			print_recursion(char *path, t_opt options)
{
	t_flist		*new_head;
	t_flist		*ptr;

	ft_miniprintf("\n%s:\n", path);
	new_head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!new_head)
	{
		perror("cannot allocate memory");
		exit(EXIT_FAILURE);
	}
	ptr = new_head;
	ft_read_dir(path, options, &new_head);
	new_head = ft_sort_flist(options, new_head);
	ft_print_flist(options, new_head);
	ft_delete_flist(options, &new_head);
	free(ptr);
}
