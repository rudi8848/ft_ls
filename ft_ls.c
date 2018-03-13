#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "libftprintf/includes/libft.h"
#include "libftprintf/includes/ft_printf.h"
#include <errno.h>

typedef	struct	s_opt
{
	int			a;
	int			l;
	int			rr;
	int			r;
	int			t;
}				t_opt;

typedef enum
{
	TYPE,
	PERM,
	LINKS,
	USER,
	GROUP,
	SIZE,
	MONTH,
	DAY,
	TIME,
	NAME,
	FIELDS
}	e_fields;

typedef enum
{
	T_DOW,
	T_MONTH,
	T_DAY,
	T_TIME,
	T_YEAR,
	T_END
}	e_time;

typedef struct		s_flist
{
	char			*name;
	char			*path;
	char			*mode;
	short			nlink;
	char			*user;
	char			*group;
	long			size;
	char			*day;
	char			*month;
	char			*time;
	struct s_flist	*next;
}					t_flist;

void		ft_read_args(char *name, t_opt *options, t_flist **head);
void		ft_get_time(struct stat buf, t_flist **head);
void		ft_read_file();
void		ft_read_dir(DIR *dirp, t_opt *options, t_flist **head);
void		ft_get_user_group(struct stat buf, t_flist **head);
int			ft_flist_count(t_flist *head);
void		ft_sort_flist(void)
{
	;
}
void		ft_print_flist(t_opt options, t_flist *head)
{
	int total;

	total = ft_flist_count(head);
	ft_printf("total %d\n", total);
	while (head->next)
	{
		if (options.l)
			ft_printf("%5s %3hi %5s %5s %5llD %4s %3s %-5.5s %-8s\n",
				head->mode, head->nlink, head->user, head->group,
				head->size, head->month, head->day, head->time,
				head->name);
		else
			ft_printf("%-15s\n", head->name);
		head = head->next;
	}
}

void		ft_clean_flist(t_flist *file)
{
	ft_strdel(&file->name);
	ft_strdel(&file->mode);
	ft_strdel(&file->user);
	ft_strdel(&file->group);
	ft_strdel(&file->day);
	ft_strdel(&file->month);
	ft_strdel(&file->time);
}

void		ft_delete_flist(t_flist **head)
{
	t_flist 	*tmp;

	while ((*head)->next)
	{
		tmp = (*head);
		*head = (*head)->next;
		ft_clean_flist(tmp);
		free(tmp);
	}
	ft_clean_flist(*head);
	free(*head);
}

int			ft_flist_count(t_flist *head)
{
	int			i;

	i = 0;
	if (!head)
		return (0);
	while (head->next)
	{
		head = head->next;
		i++;
	}
	return (i);
}

void		ft_push_fname(t_flist **head, char *name)
{
	t_flist *tmp;

	tmp = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!tmp)
	{
		perror("ft_memalloc");
		exit(1);
	}
	tmp->name = ft_strdup(name);
	tmp->next = (*head);
	(*head) = tmp;
}
/*
t_flist		*ft_get_last(t_flist *head)
{			
	if (head == NULL)
		return (NULL);
	while (head->next)
		head = head->next;							
	return head;
}

void		ft_push_back_fname(t_flist *head, char *name)
{
	t_flist	*last;
	t_flist	*tmp;

	last = ft_get_last(head);
	tmp = (t_flist*)malloc(sizeof(t_flist));
	if (!tmp)
	{
		perror(__FUNCTION__);
		exit(1);
	}
	tmp->name = name;
	tmp->next = NULL;
	last->next = tmp;
}
*/

void		ft_get_size(struct stat buf, t_flist **file)
{
	(*file)->size = buf.st_size;	
}

void		ft_get_time(struct stat buf, t_flist **file)
{
	char	**date;
	int		i;

	i = T_DOW;
	date = ft_strsplit(ctime(&buf.st_mtime), ' ');
	if (!date)
	{
		perror(__FUNCTION__);
		exit (1);
	}
	(*file)->day = ft_strdup(date[T_DAY]);
	(*file)->month = ft_strdup(date[T_MONTH]);
	(*file)->time = ft_strdup(date[T_TIME]);
	while (i < T_END)
	{
		free(date[i]);
		i++;
	}
}

void		ft_get_mode(struct stat buf, t_flist **file)
{
	(*file)->mode = ft_strnew(10);
	if (!(*file)->mode)
	{
		perror("ft_strnew");
		exit(1);
	}
	//printf("\n-------%s %s mode: %hd---------\n", __FUNCTION__, (*file)->name, buf.st_mode);
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
	//printf("\n-------%s %s mode: %s---------\n", __FUNCTION__, (*file)->name, (*file)->mode);
}

void		ft_get_links(struct stat buf, t_flist **file)
{
	(*file)->nlink = buf.st_nlink;
}

void		ft_read_link(struct stat buf, t_flist *head)
{
	;
}

void		ft_read_file(char *name, t_opt options, struct stat buf, t_flist **head)
{
	ft_push_fname(head, name);
	if (options.l)
	{
		ft_get_mode(buf, head);
		ft_get_user_group(buf, head);
		ft_get_time(buf, head);
		ft_get_size(buf, head);
		ft_get_links(buf, head);
	}
}

void		ft_read_dir(DIR *dirp, t_opt *options, t_flist **head)
{
	struct dirent 	*info;
	struct stat 	buf = {0};

	while ((info = readdir(dirp)))
	{
		/*if (ft_strequ(info->d_name, ".") || ft_strequ(info->d_name, ".."))
		 	{
		 		printf("%s\n", info->d_name);
		 		continue;
		 	}*/
		if (info->d_type & DT_REG)
		{
			//printf("\n------- reg %s---------\n", info->d_name);
			stat(info->d_name, &buf);
			ft_read_file(info->d_name, *options, buf, head);
		}
		else if (info->d_type & DT_DIR)
		{
			//printf("\n------- dir %s---------\n", info->d_name);
			lstat(info->d_name, &buf);
			ft_read_file(info->d_name, *options, buf, head);
		}
	}
}

void		ft_get_user_group(struct stat buf, t_flist **head)
{
	struct passwd	*s_user;
	struct group	*s_group;

	s_user = getpwuid(buf.st_uid);
	(*head)->user = ft_strdup(s_user->pw_name);
	s_group = getgrgid(buf.st_gid);
	(*head)->group = ft_strdup(s_group->gr_name);

}

void		ft_read_args(char *name, t_opt *options, t_flist **head)
{
	int				ret;
	DIR				*dirp;
	struct stat		buf;

	ret = stat(name, &buf);
	if (ret >= 0)
	{
		if (S_ISREG(buf.st_mode))
			ft_read_file(name, *options, buf, head);
		else if (S_ISDIR(buf.st_mode))
		{
			dirp = opendir(name);
			ft_read_dir(dirp, options, head);
			closedir(dirp);
		}
		else if (S_ISLNK(buf.st_mode))
			ft_read_link(buf, *head);
	}
	else
		perror(strerror(ret));
}

void		ft_parse_args(int argc, char **argv, t_opt *options, t_flist **head)
{
	int			i;

	i = 1;
	while (i < argc)
	{
		if (*argv[i] == '-')
		{
			++argv[i];
			while (*argv[i] && ft_isalnum(*argv[i]))
			{
				if (*argv[i] == 'a' || *argv[i] == 'l' || *argv[i] == 'R'
					|| *argv[i] == 'r' || *argv[i] == 't')
				{
					(*argv[i] == 'a') ? (options->a = 1) : (options->a = 0);
					(*argv[i] == 'l') ? (options->l = 1) : (options->l = 0);
					(*argv[i] == 'R') ? (options->rr = 1) : (options->rr = 0);
					(*argv[i] == 'r') ? (options->r = 1) : (options->r = 0);
					(*argv[i] == 't') ? (options->t = 1) : (options->t = 0);
				}
				else
				{
					ft_printf("illegal option '%c'\n", *argv[i]);
					exit (1);
				}
				argv[i]++;
			}
			i++;
	}
	if (!argv[i])
		ft_read_args(".", options, head);
	else
		ft_read_args(argv[i], options, head);
			i++;
	}
}

int			main(int argc, char **argv)
{
	t_flist		*head;
	t_opt		*options;

	options = (t_opt*)ft_memalloc(sizeof(t_opt));
	head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!options || !head)
		perror("Error");
	if (argc > 1)
		ft_parse_args(argc, argv, options, &head);
	else
		ft_read_args(".", options, &head);
	ft_sort_flist();
	ft_print_flist(*options, head);
	ft_delete_flist(&head);
	free(options);
	//system("leaks ft_ls");
	return (0);
}
