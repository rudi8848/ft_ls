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
//void		ft_read_file();
//void		ft_read_dir(/*DIR **dirp,*/ t_opt *options, t_flist **head);
void		ft_get_user_group(struct stat buf, t_flist **head);
int			ft_flist_count(t_flist *head);
void		ft_read_dir(/*DIR **dirp,*/char *name, t_opt *options, t_flist **head);
void	print_recursion(char *path, t_opt options);
void		ft_clean_flist(t_flist *file);
void		ft_sort_flist(void)
{
	;
}
void		ft_print_flist(t_opt options, t_flist *head)
{
	int total;
	t_flist *phead;

	phead = head;
	total = ft_flist_count(head);
	ft_printf("total %d\n", total);
	while (head->next)
	{
		if (options.l)
			printf("%5s %3hi %5s %5s %5ld %4s %3s %-5.5s %-8s\n",
		head->mode, head->nlink, head->user, head->group,
				head->size, head->month, head->day, head->time,
				head->name);
		else
			printf("%-15s\n", head->name);
		head = head->next;
		
	}
	if (options.rr)
		{
			while (phead->next)
			{
				struct stat buf;
				stat(phead->path, &buf);
				if (buf.st_mode & S_IFDIR && buf.st_nlink > 1 && !ft_strequ(phead->name, ".") && !ft_strequ(phead->name, ".."))
					print_recursion(phead->path, options);
				phead = phead->next;
			}
		}
}

void	print_recursion(char *path, t_opt options)
{
	printf("\n--------------%s-----------\n%s\n", __FUNCTION__, path);
	t_flist		*new_head;

	new_head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (! new_head)
	{
		perror(__FUNCTION__);
		exit(1);
	}
	ft_read_dir(path, &options, &new_head);
	ft_print_flist(options, new_head);
	ft_clean_flist(new_head);

}

void		ft_clean_flist(t_flist *file)
{
	ft_strdel(&file->name);
	ft_strdel(&file->path);
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
	//printf("\n------------------>head%p, name %s\n",&head, head->name);
	while (head->next)
	{

		head = head->next;
	//	printf("\n------------------>head %p, name %s\n",&head, head->name);
		i++;
	}
	return (i);
}

void		ft_push_fname(t_flist **head, char *path)
{
	t_flist *tmp;
	char **arr_name;
	int i = 0;

	tmp = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!tmp)
	{
		perror("ft_memalloc");
		exit(1);
	}
	tmp->path = ft_strdup(path);
	arr_name = ft_strsplit(path, '/');
	while (arr_name[i] != NULL)
		i++;
	tmp->name = ft_strdup(arr_name[i - 1]);
	while (i)
	{
		free(arr_name[i]);
		i--;
	}
	free(*arr_name);
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
	if (S_ISLNK(buf.st_mode))
		(*file)->mode[0] = 'l';
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
	//printf("\n-------%s %s mode: %s---------\n", __FUNCTION__, (*file)->name, (*file)->mode);
}

void		ft_get_links(struct stat buf, t_flist **file)
{
	(*file)->nlink = buf.st_nlink;
}

void		ft_read_link(char *name, char *path, t_flist *head)
{
		;
}

void		ft_read_file(char *path, t_opt options, struct stat buf, t_flist **head)
{
	ft_push_fname(head, path);
	if (options.l)
	{
		ft_get_mode(buf, head);
		ft_get_user_group(buf, head);
		ft_get_time(buf, head);
		ft_get_size(buf, head);
		ft_get_links(buf, head);
	}
}

void		ft_read_dir(char *name, t_opt *options, t_flist **head)
{
	struct dirent 	*info = NULL;
	struct stat 	buf = {0};
	DIR				*dirp = NULL;
	char			*path;
	char			*prefix;


	dirp = opendir(name);
	if (dirp == NULL)
	{
		perror("onendir");
		exit(1);
	}
	while ((info = readdir(dirp)))
	{
		if (info == NULL)
		{
			perror("readdir");
			exit(1);
		}
		if (! options->a)
		{
			if (ft_strequ(info->d_name, ".") || ft_strequ(info->d_name, "..") || (*info).d_name[0] == '.')
		 		continue;
		}
		prefix = ft_strjoin(name, "/");
		path = ft_strjoin(prefix, info->d_name);
		if (info->d_type &DT_LNK)
		{
			lstat(path, &buf);
			ft_read_file(path, *options, buf,head);
		}
		else if (info->d_type & DT_REG)
		{
			stat(path, &buf);
			ft_read_file(/*info->d_name*/path, *options, buf, head);
			
		}
		else if (info->d_type & DT_DIR)
		{
			stat(/*info->d_name*/path, &buf);
			ft_read_file(/*info->d_name*/path, *options, buf, head);
		}
		free(prefix);
		free(path);
	}
	closedir(dirp);
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
	//DIR				*dirp = NULL;
	struct stat		buf;

	ret = stat(name, &buf);
	if (ret >= 0)
	{
		if (S_ISLNK(buf.st_mode))
		{
			lstat(name, &buf);
			ft_read_file(name, *options, buf, head);
		}
		else if (S_ISREG(buf.st_mode))
			ft_read_file(name, *options, buf, head);
		else if (S_ISDIR(buf.st_mode))
		{
			//dirp = opendir(name);
			ft_read_dir(/*&dirp,*/name, options, head);
			//closedir(dirp);
		}
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
					if (*argv[i] == 'a')
						options->a = 1;
					if (*argv[i] == 'l')
						options->l = 1;
					if (*argv[i] == 'R')
						options->rr = 1;
					if (*argv[i] == 'r')
						options->r = 1;
					if (*argv[i] == 't')
						options->t = 1;
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
