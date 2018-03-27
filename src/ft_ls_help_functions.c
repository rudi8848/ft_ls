/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:27:09 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 13:22:51 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

size_t			ft_maxlen(t_flist *head)
{
	size_t			len;
	size_t			max;

	max = 0;
	while (head)
	{
		if ((len = ft_strlen(head->name)) > max)
			max = len;
		head = head->next;
	}
	return (max);
}

int				ft_get_rows(int total, int cols)
{
	int			rows;

	rows = total / cols;
	if (!rows)
		rows = 1;
	else if (total % cols)
		rows++;
	return (rows);
}

char			*cut_name(char *str)
{
	int			len;
	int			start;
	char		*dest;

	len = ft_strlen(str);
	start = len;
	while (start >= 0 && str[start] != '/')
		start--;
	start++;
	dest = ft_strsub(str, start, len - start);
	return (dest);
}

void			ft_check_xattr(t_flist *file)
{
	char		list[1024];
	int			ret;

	ret = listxattr(file->path, list, 1024/*, 0*/);
	if (ret > 0)
	{
		file->mode[10] = '@';
		file->mode[11] = '\0';
	}
}

void			ft_set_color(struct stat buf, t_flist **head)
{
	if (S_ISDIR(buf.st_mode))
		(*head)->color = CYAN;
	else if (S_ISLNK(buf.st_mode))
		(*head)->color = MAGENTA;
	else if (S_ISCHR(buf.st_mode))
		(*head)->color = RED;
	else if (S_ISBLK(buf.st_mode))
		(*head)->color = BLUE;
	else if (S_ISFIFO(buf.st_mode))
		(*head)->color = YELLOW;
	else if (buf.st_mode & S_IFREG && buf.st_mode & S_IXUSR)
		(*head)->color = GREEN;
	else
		(*head)->color = "";
}
