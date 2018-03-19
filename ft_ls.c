#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "libft/includes/libft.h"
#include "ft_printf.h"
#include <errno.h>

#define RESET	"\033[0m"
#define BOLD	"\033[1m"
#define RED 	"\033[1;31m"
#define GREEN 	"\033[0;32m"
#define YELLOW	"\033[1;33m"
#define MAGENTA	"\033[0;35m"
#define CYAN	"\033[1;36m"
#define WHITE	"\033[1;37m"

typedef	struct	s_opt
{
	int			a;
	int			l;
	int			rr;
	int			r;
	int			t;
}				t_opt;

typedef struct		s_flist
{
	char			*name;
	char			*path;
	char			*mode;
	short			nlink;
	char			*user;
	char			*group;
	long			size;
	long			blocks;
	char			*day;
	char			*month;
	char			*time;
	char			*color;
	time_t			mtime;
	struct s_flist	*next;
}					t_flist;

typedef int (*pfCompare)(int, int);

typedef enum
{
	SO_ASC,
	SO_DESC,
	SORT_ORDERS
} e_sort_order;


int			ft_read_args(char *name, t_opt options, t_flist **head);
void		ft_get_time(struct stat buf, t_flist **head);
//void		ft_read_file();
//void		ft_read_dir(/*DIR **dirp,*/ t_opt *options, t_flist **head);
void		ft_get_user_group(struct stat buf, t_flist **head);
int			ft_flist_count(t_flist *head);
void		ft_read_dir(/*DIR **dirp,*/char *name, t_opt options, t_flist **head);
void		print_recursion(char *path, t_opt options);
void		ft_clean_flist(t_flist **file);
t_flist		*ft_sort_flist(t_opt options, t_flist *head);
t_flist		*ft_sort_by_name(t_flist *head, pfCompare cmp);
t_flist		*ft_sort_by_mtime(t_flist *head, pfCompare cmp);
void		ft_delete_flist(t_flist **head);
long	ft_count_blocks(t_flist *head);

int			ft_cmp_ascending(int a, int b)
{
	return (a >= b);
}

int			ft_cmp_descending(int a, int b)
{
	return (a < b);
}

void		ft_print_flist(t_opt options, t_flist *head)
{
	int total = 0;
	t_flist *phead;

	phead = head;
	total = ft_count_blocks(head);
	if (options.l)
		ft_printf("total %d\n", total);
	while (head)
	{
		if (options.l)
		ft_printf("%-11s%4hi %5s %5s%7ld%4s%3s %-6s%s%s%s\n",
		head->mode, head->nlink, head->user, head->group,
				head->size, head->month, head->day, head->time, head->color,
				head->name, RESET);
		else
			ft_printf("%s%-15s%s\n", head->color, head->name, RESET);
		head = head->next;
	}
	if (options.rr)
		{
			while (phead)
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
	ft_printf("\n--------------%s-----------\n%s\n", __FUNCTION__, path);
	t_flist		*new_head;
	t_flist		*ptr;

	new_head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (! new_head)
	{
		perror(__FUNCTION__);
		exit(1);
	}
	ptr = new_head;
	ft_read_dir(path, options, &new_head);
	new_head = ft_sort_flist(options, new_head);
	ft_print_flist(options, new_head);
	ft_delete_flist(&new_head);
	free(ptr);
}

long	ft_count_blocks(t_flist *head)
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

void		ft_clean_flist(t_flist **file)
{
	ft_strdel(&(*file)->name);
	ft_strdel(&(*file)->path);
	ft_strdel(&(*file)->mode);
	ft_strdel(&(*file)->user);
	ft_strdel(&(*file)->group);
	ft_strdel(&(*file)->day);
	ft_strdel(&(*file)->month);
	ft_strdel(&(*file)->time);
}

void		ft_delete_flist(t_flist **head)
{
	t_flist 	*tmp;

	while ((*head))
	{
		tmp = (*head);
		(*head) = (*head)->next;
		ft_clean_flist(&tmp);
		free(tmp);
	}
	if (*head)
	{
		ft_clean_flist(head);
		free(head);
		(*head) = NULL;
	}
}


int			ft_flist_count(t_flist *head)
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

char 	*cut_name(char *str)
{
	int len;
	int start;
	char *dest;

	len = ft_strlen(str);
	start = len;
	while (str[start] != '/')
		start--;
	start++;
	dest = ft_strsub(str, start, len - start);
	return (dest);
}

void		ft_push_fname(t_flist **head, char *path)
{
	t_flist *tmp = NULL;

	int i = 0;

	tmp = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!tmp)
	{
		perror("cannot allocate memory");
		exit(1);
	}
	tmp->path = ft_strdup(path);
	tmp->name = cut_name(path);
	tmp->next = (*head);
	(*head) = tmp;
}

void		ft_get_size(struct stat buf, t_flist **file)
{
	(*file)->size = buf.st_size;
	(*file)->blocks = buf.st_blocks;
}

void		ft_get_time(struct stat buf, t_flist **file)
{
	char	*date;
	int		i;

	date = ctime(&buf.st_mtime);
	if (!date)
	{
		perror(__FUNCTION__);
		exit (1);
	}
	(*file)->day = ft_strsub(date, 8, 2);
	(*file)->month = ft_strsub(date, 4, 3);
	(*file)->time = ft_strsub(date, 11, 5);
}

void		ft_get_mode(struct stat buf, t_flist **file)
{
	(*file)->mode = ft_strnew(10);
	if (!(*file)->mode)
	{
		perror("Error");
		exit(1);
	}
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
}

void		ft_get_links(struct stat buf, t_flist **file)
{
	(*file)->nlink = buf.st_nlink;
}

void		ft_read_file(char *path, t_opt options, struct stat buf, t_flist **head)
{
	ft_push_fname(head, path);
	(*head)->mtime = buf.st_mtime;
	if (buf.st_mode & S_IFDIR)
		(*head)->color = CYAN;
	else if (S_ISLNK(buf.st_mode))
		(*head)->color = MAGENTA;
	else if (buf.st_mode & S_IFREG && buf.st_mode & S_IXUSR)
		(*head)->color = GREEN;
	else (*head)->color = "";
	if (options.l)
	{
		if (S_ISLNK(buf.st_mode))
		{
			;
		}
		ft_get_mode(buf, head);
		ft_get_user_group(buf, head);
		ft_get_time(buf, head);
		ft_get_size(buf, head);
		ft_get_links(buf, head);
	}
}

void		ft_read_dir(char *name, t_opt options, t_flist **head)
{
	struct dirent 	*info = NULL;
	struct stat 	buf;
	DIR				*dirp = NULL;
	char			*path;
	char			*prefix;


	dirp = opendir(name);
	if (dirp == NULL)
	{
		perror("cannot open");
		return;
	}
	while ((info = readdir(dirp)))
	{
		if (info == NULL)
		{
			perror("cannot read");
			continue;
		}
		if (!options.a && ((*info).d_name[0] == '.'))
		 		continue;
		prefix = ft_strjoin(name, "/");
		path = ft_strjoin(prefix, info->d_name);
		lstat(path, &buf);
		ft_read_file(path, options, buf,head);
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

t_flist			*ft_sort_flist(t_opt options, t_flist *head)
{
	pfCompare	cmp[SORT_ORDERS];

	cmp[SO_ASC] = ft_cmp_ascending;
	cmp[SO_DESC] = ft_cmp_descending;

	if (!options.r && !options.t)
		head = ft_sort_by_name(head, cmp[SO_ASC]);
	else if (options.r && !options.t)
		head = ft_sort_by_name(head, cmp[SO_DESC]);
	else if (options.t && !options.r)
		head = ft_sort_by_mtime(head, cmp[SO_ASC]);
	else if (options.t && options.r)
		head = ft_sort_by_mtime(head, cmp[SO_DESC]);
	return (head);
}

t_flist 		*ft_sort_by_mtime(t_flist *head, pfCompare cmp)
{
	t_flist *a = NULL;
	t_flist *b;
	t_flist *c;
    
    while (head != NULL )
    {  
        if(head->next)
        	b = head;
        else
        	return (a);
        head = head->next;
        if (a == NULL || cmp(b->mtime, a->mtime))
        {
            b->next = a;
            a = b;
        }
        else
        {
            c = a;
            while (c->next != NULL && !cmp(b->mtime, c->next->mtime))  
                  c = c->next;
            b->next = c->next;
            c->next = b;
        }
    }
    return (a);
}

t_flist 		*ft_sort_by_name(t_flist *head, pfCompare cmp)
{
    t_flist *a = NULL;
	t_flist *b;
	t_flist *c;
    
    while (head != NULL )
    {  
        if(head->next)
        	b = head;
        else
        	return (a);
        head = head->next;
        if (a == NULL || cmp(0, ft_strcmp(b->name, a->name)))
        {
            b->next = a;
            a = b;
        }
        else
        {
            c = a;
            while (c->next != NULL && !cmp(0, ft_strcmp(b->name, c->next->name)))  
                  c = c->next;
            b->next = c->next;
            c->next = b;
        }
    }
    return (a);
}

int		ft_read_args(char *name, t_opt options, t_flist **head)
{
	int			ret;
	struct stat		buf;

	ret = stat(name, &buf);
	if (ret >= 0)
	{
		if (S_ISLNK(buf.st_mode))
		{
			lstat(name, &buf);
			ft_read_file(name, options, buf, head);
		}
		else if (S_ISREG(buf.st_mode))
			ft_read_file(name, options, buf, head);
		else if (S_ISDIR(buf.st_mode))
			ft_read_dir(name, options, head);
		return (1);
	}
	else
			perror("cannot access");
		return (0);
}

t_opt		*ft_read_options(int argc, char **argv, t_opt *options)
{
	int i = 1;
	char *ptr;

	while (i < argc)
	{
		if (*argv[i] == '-')
		{
			ptr = argv[i];
			++ptr;
			while (*ptr && ft_isalnum(*ptr))
			{
				if (*ptr == 'a' || *ptr == 'l' || *ptr == 'R'
					|| *ptr == 'r' || *ptr == 't')
				{
					if (*ptr == 'a')
						options->a = 1;
					if (*ptr == 'l')
						options->l = 1;
					if (*ptr == 'R')
						options->rr = 1;
					if (*ptr == 'r')
						options->r = 1;
					if (*ptr == 't')
						options->t = 1;
				}
				else
				{
					ft_printf("illegal option '%c'\n", *ptr);
					exit (1);
				}
				ptr++;
			}
		}
			i++;
	}
	return (options);
}

int			ft_parse_args(int argc, char **argv, t_opt *options, t_flist **head)
{
	int				i;
	int				f = 0;
	int 			res = 0;
	char ***ptr = &argv;
	i = 1;

	options = ft_read_options(argc, *ptr, options);
	while (i < argc)
	{
		if (argv[i] && argv[i][0] != '-')
		{
			res = ft_read_args(argv[i], *options, head);
			f++;
		}
		i++;
	}
	if (!f)
		res = ft_read_args(".", *options, head);
	return (res);
}

int			main(int argc, char **argv)
{
	t_flist		*head = NULL;
	t_opt		*options;
	int			res;
	t_flist		*ptr;

	options = (t_opt*)ft_memalloc(sizeof(t_opt));
	head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!options || !head)
		perror("Error");
	head->next = NULL;
	ptr = head;
	if (argc > 1)
		res = ft_parse_args(argc, argv, options, &head);
	else
		res = ft_read_args(".", *options, &head);
	if (res)
	{
		head = ft_sort_flist(*options, head);
		ft_print_flist(*options, head);
	}
	ft_flist_count(head);
	ft_delete_flist(&head);
	free(options);
	free(ptr);
	//system("leaks ft_ls");
	return (0);
}
