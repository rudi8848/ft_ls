/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:27:09 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/20 15:27:19 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			ft_print_elem(t_opt options, t_flist *head)
{
	if (options.l)
		ft_printf("%-11s%4hi%10s%5s%7ld %-s%s %s%s%s\n",
			head->mode, head->nlink, head->user, head->group,
			head->size, head->date, head->color,
			head->name, RESET, head->ref);
	else
		ft_printf("%s%-15s%s\n", head->color, head->name, RESET);
}

void			ft_print_flist(t_opt options, t_flist *head)
{
	int			total;
	t_flist		*phead;
	struct stat buf;

	phead = head;
	total = ft_count_blocks(head);
	if (options.l)
		ft_printf("total %d\n", total);
	while (head)
	{
		ft_print_elem(options, head);
		head = head->next;
	}
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

	ft_printf("\n%s:\n", path);
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

long		ft_count_blocks(t_flist *head)
{
	long	total;

	total = 0;
	while (head)
	{
		total += head->blocks;
		head = head->next;
	}
	return (total);
}

void			ft_clean_flist(t_opt options, t_flist **file)
{
	ft_strdel(&(*file)->name);
	ft_strdel(&(*file)->path);
	if (options.l)
	{
		ft_strdel(&(*file)->user);
		ft_strdel(&(*file)->group);
		ft_strdel(&(*file)->date);
	}
	if ((*file)->ref[0] != '\0')
		ft_strdel(&(*file)->ref);
}

void			ft_delete_flist(t_opt options, t_flist **head)
{
	t_flist		*tmp;

	while ((*head))
	{
		tmp = (*head);
		(*head) = (*head)->next;
		ft_clean_flist(options, &tmp);
		free(tmp);
	}
	if (*head)
	{
		ft_clean_flist(options, head);
		free(head);
		(*head) = NULL;
	}
}

int				ft_flist_count(t_flist *head)
{
	int			i;

	i = 0;
	if (!head)
		return (0);
	while (head)
	{
		head = head->next;
		i++;
	}
	return (i);
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

void		ft_push_fname(t_flist **head, char *path)
{
	t_flist		*tmp;

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

void			ft_get_size(struct stat buf, t_flist **file)
{
	(*file)->size = buf.st_size;
	(*file)->blocks = buf.st_blocks;
}

void			ft_get_time(struct stat buf, t_flist **file)
{
	char		*date;
	char		*day;
	char		*month;
	char		*tim;
	char		*year;
	time_t		current;

	date = ctime(&buf.st_mtime);
	current = time(0);
	if (!date)
	{
		perror(__FUNCTION__);
		exit(EXIT_FAILURE);
	}
	day = ft_strsub(date, 8, 3);
	month = ft_strsub(date, 4, 4);
	tim = ft_strsub(date, 11, 5);
	year = ft_strsub(date, 20, 4);
	if (current - buf.st_mtime < SIX_MONTH || buf.st_mtime - current < FUTURE)
		(*file)->date = ft_strjoin(ft_strcat(month, day), tim);
	else
		(*file)->date = ft_strjoin(ft_strcat(month, day), year);
	free(day);
	free(month);
	free(tim);
	free(year);
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
}

void			ft_read_link(struct stat buf, t_flist **head)
{
	int				len;
	char			*ref;
	int				ret;
	char			*prefix;

	len = buf.st_size;
	ref = (char*)ft_memalloc(sizeof(char) * (len + 1));
	ret = readlink((*head)->path, ref, len);
	if (ret < 0)
		perror("cannot read");
	ref[len] = '\0';
	prefix = " -> ";
	(*head)->ref = ft_strjoin(prefix, ref);
	free(ref);
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

void			ft_read_file(char *pth, t_opt opt, struct stat bf, t_flist **hd)
{
	ft_push_fname(hd, pth);
	(*hd)->mtime = bf.st_mtime;
	ft_set_color(bf, hd);
	(*hd)->ref = "";
	if (opt.l)
	{
		if (S_ISLNK(bf.st_mode))
			ft_read_link(bf, hd);
		ft_get_mode(bf, hd);
		ft_get_user_group(bf, hd);
		ft_get_time(bf, hd);
		ft_get_size(bf, hd);
		(*hd)->nlink = bf.st_nlink;
	}
}

void			ft_read_dir_info(char *pref, char *fnm, t_opt opt, t_flist **hd)
{
	struct stat		buf;
	char			*path;
	
	path = ft_strjoin(pref, fnm);
	lstat(path, &buf);
	ft_read_file(path, opt, buf, hd);
	free(path);
}

void			ft_read_dir(char *name, t_opt options, t_flist **head)
{
	struct dirent	*info = NULL;
	DIR				*dirp = NULL;
	char			*prefix;

	dirp = opendir(name);
	if (dirp == NULL)
	{
		perror("cannot open");
		return ;
	}
	prefix = ft_strjoin(name, "/");
	while ((info = readdir(dirp)))
	{
		if (info == NULL)
		{
			perror("cannot read");
			continue;
		}
		if (!options.a && ((*info).d_name[0] == '.'))
			continue;
		ft_read_dir_info(prefix, info->d_name, options, head);
	}
	free(prefix);
	closedir(dirp);
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

int			ft_read_args(char *name, t_opt options, t_flist **head)
{
	int				ret;
	struct stat		buf;

	ret = lstat(name, &buf);
	if (ret >= 0)
	{
		if (S_ISDIR(buf.st_mode))
			ft_read_dir(name, options, head);
		else
			ft_read_file(name, options, buf, head);
		return (1);
	}
	else
	{
		ft_printf("./ft_ls: %s: ", name);
		perror("cannot access");
	}
	return (0);
}

int			ft_set_option(char symb, t_opt *options)
{
	if (symb == 'a' || symb == 'l' || symb == 'R'
		|| symb == 'r' || symb == 't' || symb == '1')
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
					ft_printf("./ft_ls: illegal option '%c'\n", *ptr);
					ft_printf("usage: ./ft_ls [-larRt] [file ...]\n");
					exit(EXIT_FAILURE);
				}
				ptr++;
			}
		}
		i++;
	}
	return (options);
}

int		ft_ls(t_opt options, char  *name)
{
	t_flist *head;
	t_flist	*ptr;
	int res;

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
		return (1);
	}
	free(ptr);
	return 0;
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
	system("leaks ft_ls");
	return (0);
}
