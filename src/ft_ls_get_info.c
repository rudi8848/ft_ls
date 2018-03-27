/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls_get_info.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/25 13:25:47 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 13:28:06 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"

void			ft_get_time(struct stat buf, t_flist **file)
{
	char			*date;
	char			*tim;
	char			*year;
	time_t			current;
	char			*md;

	date = ctime(&buf.st_mtime);
	current = time(0);
	if (!date)
	{
		perror(__FUNCTION__);
		exit(EXIT_FAILURE);
	}
	md = ft_strsub(date, 4, 7);
	tim = ft_strsub(date, 11, 5);
	year = ft_strsub(date, 19, 5);
	if ((current - buf.st_mtime < SIX_MONTH)
			&& (buf.st_mtime - current < FUTURE))
		(*file)->date = ft_strjoin(md, tim);
	else
		(*file)->date = ft_strjoin(md, year);
	free(md);
	free(tim);
	free(year);
}

void			ft_get_user_group(struct stat buf, t_flist **head)
{
	struct passwd	*s_user;
	struct group	*s_group;

	s_user = getpwuid(buf.st_uid);
	(*head)->user = ft_strdup(s_user->pw_name);
	s_group = getgrgid(buf.st_gid);
	(*head)->group = ft_strdup(s_group->gr_name);
}

void			ft_get_mode(struct stat buf, t_flist **file)
{
	if (S_ISLNK(buf.st_mode))
		(*file)->mode[0] = 'l';
	else if (S_ISCHR(buf.st_mode))
		(*file)->mode[0] = 'c';
	else if (S_ISFIFO(buf.st_mode))
		(*file)->mode[0] = 'p';
	else if (S_ISSOCK(buf.st_mode))
		(*file)->mode[0] = 's';
	else if (S_ISBLK(buf.st_mode))
		(*file)->mode[0] = 'b';
	else
		(*file)->mode[0] = S_IFDIR & buf.st_mode ? 'd' : '-';
	(*file)->mode[1] = S_IRUSR & buf.st_mode ? 'r' : '-';
	(*file)->mode[2] = S_IWUSR & buf.st_mode ? 'w' : '-';
	(*file)->mode[3] = S_IXUSR & buf.st_mode ? 'x' : '-';
	(*file)->mode[4] = S_IRGRP & buf.st_mode ? 'r' : '-';
	(*file)->mode[5] = S_IWGRP & buf.st_mode ? 'w' : '-';
	(*file)->mode[6] = S_IXGRP & buf.st_mode ? 'x' : '-';
	(*file)->mode[7] = S_IROTH & buf.st_mode ? 'r' : '-';
	(*file)->mode[8] = S_IWOTH & buf.st_mode ? 'w' : '-';
	(*file)->mode[9] = S_IXOTH & buf.st_mode ? 'x' : '-';
	(*file)->mode[10] = '\0';
	ft_check_xattr((*file));
}

void			ft_push_fname(t_flist **head, char *path)
{
	t_flist			*tmp;

	tmp = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!tmp)
	{
		perror("cannot allocate memory");
		exit(EXIT_FAILURE);
	}
	tmp->path = ft_strdup(path);
	tmp->name = cut_name(path);
	tmp->next = (*head);
	(*head) = tmp;
}
